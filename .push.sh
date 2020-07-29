msg=$1
if [ -n "$msg" ]; then
  git add .
  git commit -m "${msg}"
  git push
  echo "git status is:"
  git status
  echo "finish add, commit, push"
else
  echo "please add commit content"
fi
