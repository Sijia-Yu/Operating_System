#do cmd command in sh file

while true
do
	read -p "Please input your command: " userCommand

	#check invalid input
	str=`echo "$userCommand" | tr -d ' [A-Za-z]///.'`
	if [ -n "$str" ] 
	then
		echo "Invalid input!"
		continue
	fi

	var=${userCommand%\ *}
	content=${userCommand#*\ }

	#match command
	case $var in
		"ls")
			ls
			;;
		"pwd")
			pwd
			;;
		"cd")
			cd $content
			;;
		"hostname")
			hostname
			;;
		"who")
			who
			;;
	esac
done
