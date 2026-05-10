# =============================================================================
# export-snapshots.ps1  -  Exporteer per opdracht-tag een schone snapshot-map.
# =============================================================================
# Gebruik:
#   .\scripts\export-snapshots.ps1                    # alle tags 'opdracht-*'
#   .\scripts\export-snapshots.ps1 -Tag opdracht-3    # alleen deze tag
#   .\scripts\export-snapshots.ps1 -Zip               # ook .zip per tag
#
# Resultaat:
#   snapshots/
#   ├── opdracht-1/   <- complete code-toestand bij die tag
#   ├── opdracht-2/
#   └── ...
#   en optioneel: snapshots/opdracht-N.zip
#
# Geen build/, geen .git/ - alleen bron-files. Op te leveren of te zippen
# voor de docent.
# =============================================================================

param(
    [string]$Tag = "all",
    [switch]$Zip
)

$ErrorActionPreference = "Stop"

$repoRoot     = Split-Path $PSScriptRoot -Parent
$snapshotRoot = Join-Path $repoRoot "snapshots"

if (-not (Test-Path $snapshotRoot)) {
    New-Item -ItemType Directory $snapshotRoot | Out-Null
}

# --- Welke tags exporteren ---------------------------------------------------
$tags = if ($Tag -eq "all") {
    git -C $repoRoot tag --list "opdracht-*" | Sort-Object
} else {
    @($Tag)
}

if (-not $tags -or $tags.Count -eq 0) {
    Write-Warning "Geen tags gevonden (verwacht: 'opdracht-*'). Heb je al getagd met scripts/tag-opdracht.ps1?"
    exit 0
}

# --- Per tag exporteren ------------------------------------------------------
foreach ($t in $tags) {
    $target = Join-Path $snapshotRoot $t

    if (Test-Path $target) {
        Write-Host ">> bestaande $t verwijderen" -ForegroundColor DarkGray
        Remove-Item -Recurse -Force $target
    }
    New-Item -ItemType Directory $target | Out-Null

    Write-Host ">> exporteer $t" -ForegroundColor Cyan

    # git archive maakt een zip vanuit de exacte commit van de tag
    $tmpZip = Join-Path $env:TEMP "$t.zip"
    git -C $repoRoot archive --format=zip --output $tmpZip $t

    # Uitpakken
    Expand-Archive -Path $tmpZip -DestinationPath $target -Force

    # Eventueel zip behouden voor in te leveren
    if ($Zip) {
        $finalZip = Join-Path $snapshotRoot "$t.zip"
        Move-Item -Force $tmpZip $finalZip
        Write-Host "   -> $finalZip"
    } else {
        Remove-Item $tmpZip
    }

    Write-Host "   -> $target"
}

Write-Host ""
Write-Host "OK Snapshots klaar in: $snapshotRoot" -ForegroundColor Green
