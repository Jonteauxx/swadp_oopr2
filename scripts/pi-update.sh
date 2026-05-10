#!/usr/bin/env bash
# =============================================================================
# pi-update.sh - wordt door deploy.ps1 op de Pi uitgevoerd na een push.
# =============================================================================
# Argumenten:
#   $1 = doel-branch (default: main)
#
# Stappen:
#   1. fetch alle remote-branches
#   2. checkout doel-branch (maakt aan als hij lokaal nog niet bestaat)
#   3. reset --hard naar origin/<branch>   (deploy-target = mirror van origin)
#   4. cmake configureren als build/ nog niet bestaat
#   5. cmake build (alle cores)
#   6. ctest (alle unit-tests, output bij falen)
# =============================================================================

set -euo pipefail

REPO_DIR="${HOME}/swadp_oopr2"
BRANCH="${1:-main}"

cd "${REPO_DIR}"

echo ">> doel-branch:  ${BRANCH}"
echo ">> git fetch origin"
git fetch origin

# Switch naar de gewenste branch (maak lokaal aan als hij er nog niet is).
if git rev-parse --verify "${BRANCH}" >/dev/null 2>&1; then
    git checkout "${BRANCH}"
else
    git checkout -b "${BRANCH}" "origin/${BRANCH}"
fi

echo ">> reset --hard origin/${BRANCH}"
git reset --hard "origin/${BRANCH}"

if [ ! -d build ]; then
    echo ">> eerste keer: cmake configureren"
    cmake -B build -S .
fi

echo ">> cmake --build build"
cmake --build build -j"$(nproc)"

echo ""
echo ">> unit-tests draaien (ctest)"
cd build
if ctest --output-on-failure; then
    echo ""
    echo "============================================================"
    echo "  BUILD OK + TESTS GESLAAGD"
    echo "  branch:     ${BRANCH}"
    echo "  executable: ${REPO_DIR}/build/gebouw"
    echo "============================================================"
else
    echo ""
    echo "============================================================"
    echo "  BUILD OK, maar TESTS GEFAALD - zie hierboven"
    echo "============================================================"
    exit 1
fi
