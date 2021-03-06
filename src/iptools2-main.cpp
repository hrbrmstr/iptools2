// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(AsioHeaders)]]

#include <Rcpp.h>
#include <stdio.h>
#include <asio.hpp>
#include <asio/ip/network_v4.hpp>
#include <asio/ip/network_v6.hpp>

using namespace Rcpp;

namespace ip = asio::ip;

static double power_lookup_tbl(unsigned pow) {
  static double pow2[] = {
    1.0,
    2.0,
    4.0,
    8.0,
    16.0,
    32.0,
    64.0,
    128.0,
    256.0,
    512.0,
    1024.0,
    2048.0,
    4096.0,
    8192.0,
    16384.0,
    32768.0,
    65536.0,
    131072.0,
    262144.0,
    524288.0,
    1048576.0,
    2097152.0,
    4194304.0,
    8388608.0,
    16777216.0,
    33554432.0,
    67108864.0,
    134217728.0,
    268435456.0,
    536870912.0,
    1073741824.0,
    2147483648.0,
    4294967296.0,
    8589934592.0,
    17179869184.0,
    34359738368.0,
    68719476736.0,
    137438953472.0,
    274877906944.0,
    549755813888.0,
    1099511627776.0,
    2199023255552.0,
    4398046511104.0,
    8796093022208.0,
    17592186044416.0,
    35184372088832.0,
    70368744177664.0,
    140737488355328.0,
    281474976710656.0,
    562949953421312.0,
    1125899906842624.0,
    2251799813685248.0,
    4503599627370496.0,
    9007199254740992.0,
    18014398509481984.0,
    36028797018963968.0,
    72057594037927936.0,
    144115188075855872.0,
    288230376151711744.0,
    576460752303423488.0,
    1152921504606846976.0,
    2305843009213693952.0,
    4611686018427387904.0,
    9223372036854775808.0,
    18446744073709551616.0,
    36893488147419103232.0,
    73786976294838206464.0,
    147573952589676412928.0,
    295147905179352825856.0,
    590295810358705651712.0,
    1180591620717411303424.0,
    2361183241434822606848.0,
    4722366482869645213696.0,
    9444732965739290427392.0,
    18889465931478580854784.0,
    37778931862957161709568.0,
    75557863725914323419136.0,
    151115727451828646838272.0,
    302231454903657293676544.0,
    604462909807314587353088.0,
    1208925819614629174706176.0,
    2417851639229258349412352.0,
    4835703278458516698824704.0,
    9671406556917033397649408.0,
    19342813113834066795298816.0,
    38685626227668133590597632.0,
    77371252455336267181195264.0,
    154742504910672534362390528.0,
    309485009821345068724781056.0,
    618970019642690137449562112.0,
    1237940039285380274899124224.0,
    2475880078570760549798248448.0,
    4951760157141521099596496896.0,
    9903520314283042199192993792.0,
    19807040628566084398385987584.0,
    39614081257132168796771975168.0,
    79228162514264337593543950336.0,
    158456325028528675187087900672.0,
    316912650057057350374175801344.0,
    633825300114114700748351602688.0,
    1267650600228229401496703205376.0,
    2535301200456458802993406410752.0,
    5070602400912917605986812821504.0,
    10141204801825835211973625643008.0,
    20282409603651670423947251286016.0,
    40564819207303340847894502572032.0,
    81129638414606681695789005144064.0,
    162259276829213363391578010288128.0,
    324518553658426726783156020576256.0,
    649037107316853453566312041152512.0,
    1298074214633706907132624082305024.0,
    2596148429267413814265248164610048.0,
    5192296858534827628530496329220096.0,
    10384593717069655257060992658440192.0,
    20769187434139310514121985316880384.0,
    41538374868278621028243970633760768.0,
    83076749736557242056487941267521536.0,
    166153499473114484112975882535043072.0,
    332306998946228968225951765070086144.0,
    664613997892457936451903530140172288.0,
    1329227995784915872903807060280344576.0,
    2658455991569831745807614120560689152.0,
    5316911983139663491615228241121378304.0,
    10633823966279326983230456482242756608.0,
    21267647932558653966460912964485513216.0,
    42535295865117307932921825928971026432.0,
    85070591730234615865843651857942052864.0,
    170141183460469231731687303715884105728.0,
  };
  if (pow <= 127)
    return pow2[pow];
  return -1.0;
}

enum empties_t { empties_ok, no_empties };

template <typename Container>
Container& split(
    Container&                            result,
    const typename Container::value_type& s,
    const typename Container::value_type& delimiters,
    empties_t                      empties = empties_ok )
{
  result.clear();
  size_t current;
  size_t next = -1;
  do
  {
    if (empties == no_empties)
    {
      next = s.find_first_not_of( delimiters, next + 1 );
      if (next == Container::value_type::npos) break;
      next -= 1;
    }
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
  }
  while (next != Container::value_type::npos);
  return result;
}

//' Retrieve range boundaries for a character vector of IPv4/IPv6 CIDRs
//'
//' @param cidr a character vector of IPv4/IPv6 CIDRs
//' @return data frame with original input, canonical CIDR, start/end (chr) of range and number of hosts.
//' @note the `n_hosts` column is a numeric vector since R cannot handle 128-bit integers easily and
//'       v4 & v6 are co-mingled.
//' @export
//' @examples
//' range_boundaries(
//'   c("2001:4801::/32", "2001:0db8::/127", "10.1.10.0/24", "2001:0db8::/119", "wat")
//' )
// [[Rcpp::export]]
DataFrame range_boundaries(std::vector < std::string > cidr) {

  StringVector cstart(cidr.size());
  StringVector cend(cidr.size());
  StringVector canon(cidr.size());
  NumericVector sz(cidr.size());

  for (R_xlen_t i = 0; i < cidr.size(); i++) {

    if (cidr[i].find(":") != std::string::npos) {
      try {
        ip::network_v6 net = ip::make_network_v6(cidr[i]);
        ip::address_v6_range rng = net.hosts();
        sz[i] = power_lookup_tbl(128 - net.prefix_length());
        canon[i] = net.canonical().to_string();
        cstart[i] = (rng.begin())->to_string();
        cend[i] = (rng.end())->to_string();
      } catch (...) {
        sz[i] = NA_REAL;
        cstart[i] = NA_STRING;
        cend[i] = NA_STRING;
        canon[i] = NA_STRING;
      }
    } else {
      try {
        ip::network_v4 net = ip::make_network_v4(cidr[i]);
        ip::address_v4_range rng = net.hosts();
        if (net.prefix_length() >= 31) {
          sz[i] = power_lookup_tbl(32 - net.prefix_length());
        } else {
          sz[i] = (1 << (32 - net.prefix_length())) - 2;
        };
        canon[i] = net.canonical().to_string();
        cstart[i] = (rng.begin())->to_string();
        cend[i] = (rng.end())->to_string();
      } catch(...) {
        sz[i] = NA_REAL;
        cstart[i] = NA_STRING;
        cend[i] = NA_STRING;
        canon[i] = NA_STRING;
      }
    }

  }

  return(
    DataFrame::create(
      _["cidr"] = cidr,
      _["canonical"] = canon,
      _["start"] = cstart,
      _["end"] = cend,
      _["n_hosts"] = sz,
      _["stringsAsFactors"] = false
    )
  );

}



// iptools2:::range_boundaries(c("2001:4801::/32", "2001:0db8::/127", "10.1.10.0/24", "2001:0db8::/119", "steve"))












