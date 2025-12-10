#!/bin/bash
# Push script for FireBird BBS MIPS32LE build workflow

set -e

echo "================================================"
echo "  FireBird BBS - Push to GitHub"
echo "================================================"
echo ""

cd "$(dirname "$0")"

# Check git status
echo "Checking git status..."
git status

echo ""
echo "Ready to push the following commits:"
git log --oneline origin/main..HEAD

echo ""
read -p "Push to GitHub? (y/n) " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Pushing to GitHub..."
    git push origin main
    
    echo ""
    echo "鉁� Successfully pushed to GitHub!"
    echo ""
    echo "The GitHub Actions workflow will now:"
    echo "  1. Cross-compile for MIPS32 Little-Endian"
    echo "  2. Build termcap, ncurses, libxcrypt from source"
    echo "  3. Build libBBS and all BBS executables"
    echo "  4. Package binaries into bbs-mipsle-binaries.tar.gz"
    echo "  5. Upload to repository and artifacts"
    echo ""
    echo "Monitor the build at:"
    echo "  https://github.com/$(git remote get-url origin | sed 's/.*github.com[:/]\(.*\)\.git/\1/')/actions"
    echo ""
else
    echo "Push cancelled."
    exit 1
fi
