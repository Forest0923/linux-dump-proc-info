#!/bin/bash
set -e

source lib/inquirer.sh

select_feature() {
	mode=('Dump kernel objects' 'VA -> PA translation')
	list_input "Select feature:" mode selected_feature
}

get_target_pid() {
	while [ true ]; do
		text_input "Specify target PID or process name:" pid_name
		if echo $pid_name | grep "^\s*[0-9]\+\s*$" >/dev/null ; then
			target_pid=$pid_name
		else
			target_pid=`pidof -s $pid_name || echo ""`
		fi

		if echo $target_pid | grep "^\s*[0-9]\+\s*$" >/dev/null ; then
			break
		fi
	done
}

dump_object() {
}

vapa_translate() {
}

# main

select_feature
get_target_pid

if [ $selected_feature == 'Dump kernel objects' ]; then
	dump_objects $target_pid
elif [ $selected_feature == 'VA -> PA translation' ]
	vapa_translate $target_pid
fi

