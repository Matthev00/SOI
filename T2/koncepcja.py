import random
import time


def time_group(group):
    if group == "A":
        return 1
    elif group == "B":
        return 2
    elif group == "C":
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
    for i in range(len(process_list) - 1):
        process_list[i].next = process_list[i + 1]
    return tail, head


def next_groupss(group):
    if group == "A":
        return "B"
    elif group == "B":
        return "C"
    elif group == "C":
        return "A"


def processes_init():
    process_list = []
    for i in range(5):
        process_list.append(Process("A", i * 3 + 1))
        process_list.append(Process("B", i * 3 + 2))
        process_list.append(Process("C", i * 3 + 3))
    random.shuffle(process_list)
    return process_list


def hold_proc(process, _time):
    for _ in range(_time):
        time.sleep(0.1)


def sched(tail, head, iteration_num):
    curr = head
    next_group = next_groupss(curr.group)
    while iteration_num > 0:
        while iteration_num > 0:
            if curr == tail:
                curr = head
                next_group = next_groupss(next_group)
                break
            if curr.next.group == next_group:
                next = curr.next
                curr.next = curr.next.next
                next.next = head.next
                head.next = next
                tail.next = head
                tail = tail.next
                head = next
                next_group = next_groupss(next_group)
                print(head.group)
            else:
                curr = curr.next
            iteration_num -= 1


def main():
    process_list = processes_init()
    tail, head = link_list(process_list)

    for process in process_list:
        print(process.id, process.group)
    sched(tail, head, 100)


if __name__ == "__main__":
    main()
