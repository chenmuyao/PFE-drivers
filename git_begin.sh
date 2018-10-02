echo "Please enter the branch:"
read branch

git fetch
git pull origin $branch
