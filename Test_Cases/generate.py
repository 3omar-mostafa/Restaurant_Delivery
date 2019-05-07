# Generate Random Test Cases:
import random
rand = random.randint
def write(line):
    print(line, file=f)

####################################################################################################

#orderTypeList = ['N', 'N', 'N']
#orderTypeList = ['N', 'F', 'N']
#orderTypeList = ['N', 'N', 'V']
#orderTypeList = ['N', 'F', 'V']
orderTypeList = ['V', 'F', 'V']
regionList = ['A', 'B', 'C', 'D']

name, num, motors = input("<Filename> <Number of Arrival Events> <Number of Motors>\n").split()
name = name + ".txt"
num = int(num)
motors = int(motors)
f = open(name, "w")


# for reg in range(4):
#     write("{} 0 0".format(motors))
#     for i in range(1, motors + 1):
#         write("{} {}".format(motors * reg + i, 4 * i))
#     write("")

# for reg in range(4):
#     write("{} {} 0".format(motors - 1, 1))
#     for i in range(1, motors + 1):
#         write("{} {}".format(motors * reg + i, 4 * i))
#     write("")

for reg in range(4):
    write("{} {} {}".format(1, 1, motors - 2))
    for i in range(1, motors + 1):
        write("{} {}".format(motors * reg + i, 4 * i))
    write("")

write("10\n\n{}".format(num))
eventList = []


for i in range(num):

    #data = "R {} ".format(1 + i // 10)
    data = "R {} ".format(1)
    data = data + orderTypeList[rand(0, 2)] + "\t {} {} {} ".format(i + 1, rand(4,  8 * motors), rand(1, 25) * 100)
    data = data + regionList[rand(0, 3)]

    eventList.append(data)
    #write(data)

for i in range(num - 1):
    minIdx = i
    for j in range(i + 1, num):
        if eventList[j][-1] < eventList[minIdx][-1]:
            minIdx = j
    eventList[i], eventList[minIdx] = eventList[minIdx], eventList[i]

for i in range(num):
    wordlist = eventList[i].split()
    wordlist.pop(3)
    wordlist.insert(3, str(i + 1))
    eventList[i] = str(" ").join(wordlist)
    write(eventList[i])
f.close()
