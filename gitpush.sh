echo "git push start"
echo "current git status is:"
git status
read -p "git push yes(y) or no:  " isPush
if [ $isPush = "y" -o $isPush = "yes" ]; then
  git add .
  read -p "Please input the commit messge:  " msg
  if [ -n "$msg" ]; then
    git commit -m "${msg}"
    git push
    echo "git push success!"
  else
    echo "please add commit content"
  fi
else
  echo "git push fail."
fi
