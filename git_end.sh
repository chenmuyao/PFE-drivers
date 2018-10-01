echo "Please leave a message: "
read message

git add -A
git commit -m "$message"

echo "Push or not?(y/n)"

read push

if [ $push == "y" || $push == "Y" ] ;
then 
    echo "To which branch?"
    read branch
else exit
fi

git push origin $branch
