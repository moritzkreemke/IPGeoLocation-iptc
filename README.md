# IPGeoLocation (iptc)
[![CMake](https://github.com/Get-Smart1/IPGeoLocation-iptc/actions/workflows/cmake.yml/badge.svg)](https://github.com/Get-Smart1/IPGeoLocation-iptc/actions/workflows/cmake.yml)

A small CLI Tool to determine the Country of an IP-Address.

## Usage
To check a single IP Address:
```bash
iptc 77.183.72.247
> The IP 77.183.72.247 comes from DE
```
You can also display all IP-Addresses found in a File:
```bash
iptc /var/log/auth.log
> The IP 49.36.107.13 comes from IN
> The IP 141.98.10.248 comes from LT
> The IP 199.19.225.170 comes from US
> The IP 45.144.225.69 comes from NL
> ...
```
You can also use the parameter -f to follow a stream and see from where People trying to connect to your ssh Server
```bash
tail-f /var/log/auth.log | iptc -f -s
> DE: 15; NL: 7; US: 3; CN: 2; LT: 2
```

## Background

I needed to write for my second CS-Semester a simple C-Program. Because
I was always fascinated while renting a Server, how many people are trying to log in via SSH.
That's why, I  wanted to have some more insights (like from where the most requests are).
This is my first written C Program, so it's far from perfect and even has some memory leaks which I
have to fix in the future. But for now, it's doing its job. 