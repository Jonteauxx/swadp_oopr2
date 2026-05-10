# =============================================================================
# deploy.ps1  -  Een-commando deploy: commit -> GitHub -> Pi pull+build
# =============================================================================
# Gebruik:
#   .\deploy.ps1 "commit-bericht hier"
#   .\deploy.ps1                            (gebruikt timestamp als bericht)
#   .\deploy.ps1 -SkipCommit                (alleen Pi opnieuw bouwen)
#   .\deploy.ps1 -OnlyCommit                (alleen committen + pushen, geen Pi)
# =============================================================================

param(
    [Parameter(Position=0)]
    [string]$Message = "",

    [switch]$SkipCommit,
    [switch]$OnlyCommit
)

# === Configuratie ============================================================
$PiHost = "rpi@192.168.137.62"
$PiRepo = "~/swadp_oopr2"
$UpdateScript = "scripts/pi-update.sh"
# =============================================================================

$ErrorActionPreference = "Stop"

function Write-Step($msg)   { Write-Host ">> $msg" -ForegroundColor Cyan }
function Write-Ok($msg)     { Write-Host "OK $msg"  -ForegroundColor Green }
function Write-Err($msg)    { Write-Host "!! $msg"  -ForegroundColor Red }

# --- 1. Commit + push naar GitHub --------------------------------------------
if (-not $SkipCommit) {
    Write-Step "git status check"
    $status = git status --porcelain
    if ($status) {
        if (-not $Message) {
            $Message = "wip $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
            Write-Host "   geen bericht meegegeven, gebruik: '$Message'" -ForegroundColor Yellow
        }

        Write-Step "git add -A"
        git add -A

        Write-Step "git commit"
        git commit -m $Message
        if ($LASTEXITCODE -ne 0) { Write-Err "commit faalde"; exit 1 }
    } else {
        Write-Host "   geen wijzigingen, sla commit over" -ForegroundColor Yellow
    }

    Write-Step "git push origin"
    $branch = git branch --show-current
    git push -u origin $branch
    if ($LASTEXITCODE -ne 0) { Write-Err "push faalde"; exit 1 }
    Write-Ok "GitHub bijgewerkt"
}

if ($OnlyCommit) {
    Write-Ok "Klaar (OnlyCommit)"
    exit 0
}

# --- 2. SSH naar Pi: pull + build (op DEZELFDE branch als hier) -------------
$branch = git branch --show-current
Write-Step "SSH naar $PiHost - branch '$branch' - pull + build"
ssh $PiHost "cd $PiRepo && bash $UpdateScript '$branch'"
if ($LASTEXITCODE -ne 0) {
    Write-Err "Pi build faalde"
    exit 1
}

Write-Ok "Deploy compleet."
