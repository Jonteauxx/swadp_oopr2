#!/usr/bin/env bash
# =============================================================================
# pi-setup.sh - eenmalige setup van de Raspberry Pi.
# =============================================================================
# Uitvoeren op de Pi:
#   bash scripts/pi-setup.sh
# =============================================================================

set -euo pipefail

echo ">> apt update + upgrade"
sudo apt update
sudo apt upgrade -y

echo ">> ontwikkel-tooling installeren"
sudo apt install -y \
    build-essential \
    cmake \
    git \
    pkg-config

echo ">> Qt 6 installeren"
sudo apt install -y \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    libqt6widgets6

echo ">> pigpio (GPIO/PWM library + daemon)"
sudo apt install -y pigpio python3-pigpio

echo ">> pigpiod inschakelen zodat we niet als root hoeven te draaien"
sudo systemctl enable pigpiod
sudo systemctl start pigpiod

echo ">> Doxygen + Graphviz voor documentatie-portfolio"
sudo apt install -y doxygen graphviz

echo ">> Google Test voor unit-tests"
sudo apt install -y libgtest-dev libgmock-dev

echo ""
echo "============================================================"
echo "Setup compleet."
echo "Status pigpiod: $(systemctl is-active pigpiod)"
echo "Qt versie:      $(qmake6 --version 2>/dev/null | head -1 || echo 'qmake6 niet gevonden')"
echo "Doxygen:        $(doxygen --version)"
echo "============================================================"
