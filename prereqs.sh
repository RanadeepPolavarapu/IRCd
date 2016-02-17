#!/usr/bin/env bash

echo "[INFO]: Installing libssl-dev for UnrealIRCd SSL connections"
apt-get -y install libssl-dev

echo "[INFO]: Installing gmake for building Anope services"
apt-get -y install gmake
