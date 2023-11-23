import random
import time

def time_group(group):
    if(type == "A"):
        return 1
    elif(type == "B"):
        return 2
    elif(type == "C"):
        return 4


class Process:
    def __init__(self, group, id):
        self.id = id
        self.group = group
        self.time = time_group(group)
        self.next = None


def link_list(process_list):
    tail = process_list[-1]
    head = process_list[0]
    for i in range(len(process_list)-1):
        process_list[i].next = process_list[i+1]
    return tail, head


def next_group(group):
    if(type == "A"):
        return "B"
    elif(type == "B"):
        return "C"
    elif(type == "C"):
        return "A"


def processes_init():
    process_list = []
    for i in range(5):
        process_a = Process("A", i*3+ 1)
        process_b = Process("B", i*3+ 2)
        process_c = Process("C", i*3+ 3)
        process_list.append(process_a)
        process_list.append(process_b)
        process_list.append(process_c)
    random.shuffle(process_list)
    return process_list


def hold_proc(process, _time):
    for _ in range(_time):
        time.sleep(0.1)


def sched(tail, head, iteration_num, times):
    start = time.time()
    next_types = ["we/wy", "obl", "inter"]
    curr = head
    for next_type in next_types:
	    while(iteration_num > 0):
		if(curr == tail):
		    break
		if(curr.next.type == next_type):
		    pick_proc(curr.next, curr.next.time)
		    next = curr.next
		    curr.next = curr.next.next
		    next.next = head.next
		    head.next = next
		    tail.next = head
		    tail = tail.next
		    head = next
		    next_type = next_process_type(next_type)
		    print(f"GROUP: {head.group}")
		    curr = head
		else: 
		    curr = curr.next
		iteration_num -= 1
      
      

process_list = processes_init()
tail, head = linked_list_init(process_list)

for process in process_list:
    print(process.id, process.type)
sched(tail, head, 1000, times)

