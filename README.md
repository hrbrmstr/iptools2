
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-67%25-lightgrey.svg)
[![Linux build
Status](https://travis-ci.org/hrbrmstr/iptools2.svg?branch=master)](https://travis-ci.org/hrbrmstr/iptools2)  
![Minimal R
Version](https://img.shields.io/badge/R%3E%3D-3.2.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# iptools2

A WIP Re-imagining of {iptools}

## Description

A WIP Re-imagining of {iptools}

## What’s Inside The Tin

The following functions are implemented:

## Installation

``` r
remotes::install_git("https://git.rud.is/hrbrmstr/iptools2.git")
# or
remotes::install_git("https://git.sr.ht/~hrbrmstr/iptools2")
# or
remotes::install_gitlab("hrbrmstr/iptools2")
# or
remotes::install_bitbucket("hrbrmstr/iptools2")
# or
remotes::install_github("hrbrmstr/iptools2")
```

NOTE: To use the ‘remotes’ install options you will need to have the
[{remotes} package](https://github.com/r-lib/remotes) installed.

## Usage

``` r
library(iptools2)

# current version
packageVersion("iptools2")
## [1] '0.1.0'
```

``` r
iptools2:::range_boundaries(
  c("2001:4801::/32", "2001:0db8::/127", "10.1.10.0/24", "2001:0db8::/119", "steve")
)
##              cidr      canonical       start           end      n_hosts
## 1  2001:4801::/32 2001:4801::/32 2001:4801::   2001:4802:: 7.922816e+28
## 2 2001:0db8::/127 2001:db8::/127  2001:db8::   2001:db8::2 2.000000e+00
## 3    10.1.10.0/24   10.1.10.0/24   10.1.10.1   10.1.10.255 2.540000e+02
## 4 2001:0db8::/119 2001:db8::/119  2001:db8:: 2001:db8::200 5.120000e+02
## 5           steve           <NA>        <NA>          <NA>           NA
```

## iptools2 Metrics

| Lang | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :--- | -------: | ---: | --: | ---: | ----------: | ---: | -------: | ---: |
| C++  |        2 | 0.29 | 242 | 0.92 |          33 | 0.62 |        7 | 0.12 |
| Rmd  |        1 | 0.14 |  11 | 0.04 |          16 | 0.30 |       30 | 0.54 |
| R    |        4 | 0.57 |   9 | 0.03 |           4 | 0.08 |       19 | 0.34 |

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
