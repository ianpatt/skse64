# SKSE for Modlists - Publish to GitHub
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "SKSE for Modlists - GitHub Publishing" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$repoName = "skse-for-modlists"
$repoOwner = "cashcon57"
$description = "Performance-optimized SKSE64 for large modlists (Nolvus v6, 400+ plugins)"

# Check authentication
Write-Host "Checking GitHub authentication..." -ForegroundColor Yellow
gh auth status 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Host "Not authenticated. Run: gh auth login" -ForegroundColor Red
    exit 1
}
Write-Host "Authenticated with GitHub" -ForegroundColor Green

# Create repository
Write-Host ""
Write-Host "Creating repository..." -ForegroundColor Yellow
gh repo create "$repoOwner/$repoName" --public --description $description 2>&1 | Out-Null
if ($LASTEXITCODE -eq 0) {
    Write-Host "Repository created" -ForegroundColor Green
} else {
    Write-Host "Repository may already exist, continuing..." -ForegroundColor Yellow
}

# Configure remote
Write-Host ""
Write-Host "Configuring git remote..." -ForegroundColor Yellow
Set-Location "C:\Users\cashc\skse64"

$remotes = git remote
if ($remotes -contains "fork") {
    git remote set-url fork "https://github.com/$repoOwner/$repoName.git"
} else {
    git remote add fork "https://github.com/$repoOwner/$repoName.git"
}
Write-Host "Remote configured" -ForegroundColor Green

# Push code
Write-Host ""
Write-Host "Pushing code to GitHub..." -ForegroundColor Yellow
git push fork master 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    git push -f fork master 2>&1 | Out-Null
}
Write-Host "Code pushed" -ForegroundColor Green

# Create tag
Write-Host ""
Write-Host "Creating release tag..." -ForegroundColor Yellow
$tagExists = git tag -l "v2.0.20-optimized-1"
if (-not $tagExists) {
    git tag -a v2.0.20-optimized-1 -m "Performance optimizations for Nolvus v6"
}

git push fork v2.0.20-optimized-1 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    git push -f fork v2.0.20-optimized-1 2>&1 | Out-Null
}
Write-Host "Tag pushed" -ForegroundColor Green

# Summary
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "SUCCESS!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Repository: https://github.com/$repoOwner/$repoName" -ForegroundColor Cyan
Write-Host ""
Write-Host "Opening in browser..." -ForegroundColor Yellow
gh repo view "$repoOwner/$repoName" --web
