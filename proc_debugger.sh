#!/bin/bash
set -e

source lib/inquirer.sh

select_feature() {
	mode=('Dump kernel objects' 'VA -> PA translation')
	list_input "Select a feature:" mode selected_feature
}

get_target_pid() {
	while [ true ]; do
		text_input "Specify target PID or process name:" pid_name
		if echo $pid_name | grep "^\s*[0-9]\+\s*$" >/dev/null ; then
			target_pid=$pid_name
		else
			target_pid=`pidof -s $pid_name || echo ""`
		fi
		echo "PID = $target_pid"

		if echo $target_pid | grep "^\s*[0-9]\+\s*$" >/dev/null ; then
			break
		fi
	done
}

dump_objects() {
	action="Reconf"
	while [ true ]; do
		if [ $action == "Reconf" ]; then
			objects=('CR3' 'VMA' 'Memory-region' 'TLS-offset' 'Files')
			checkbox_input "Select objects:" objects selected_objects
		fi
		obj_mask=0
		for i in ${selected_objects[@]}; do
			case $i in
				"CR3")
					((obj_mask=$obj_mask|0x01)) ;;
				"VMA")
					((obj_mask=$obj_mask|0x02)) ;;
				"Memory-region")
					((obj_mask=$obj_mask|0x04)) ;;
				"TLS-offset")
					((obj_mask=$obj_mask|0x08)) ;;
				"Files")
					((obj_mask=$obj_mask|0x10)) ;;
			esac
		done
		if [ $obj_mask == 0 ]; then
			continue
		fi
		make -s -C src/ dump_objects PID=$1 OBJ_MASK=$obj_mask UPTIME=`sudo cat /proc/uptime | awk '{print $1}'`

		menu=('Rerun' 'Reconf' 'Quit')
		list_input "Next action:" menu action

		if [ $action == "Quit" ]; then
			break
		fi
	done
}

va2pa() {
	echo "Enter virtual address (e.g. 0x7ffff7dac000)"
	while [ true ]; do
		text_input "" addr
		if echo "$addr" | grep "^0x[0-9a-fA-F]\+$" > /dev/null ; then
			make -s -C src/ va2pa PID=$1 ADDR=$addr UPTIME=`sudo cat /proc/uptime | awk '{print $1}'`
		fi
		if [ "$addr" == "quit" -o "$addr" == "q" -o "$addr" == "exit" ]; then
			break
		fi
	done
}

# main

get_target_pid
select_feature

if [ "$selected_feature" == "Dump kernel objects" ]; then
	dump_objects $target_pid
elif [ "$selected_feature" == "VA -> PA translation" ]; then
	va2pa $target_pid
fi
