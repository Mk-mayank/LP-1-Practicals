result = []
output = []
LC = 0
input_data = []

# Reading input data from file
with open('input2.txt', 'r') as file:
    lines = file.readlines()

# Strip each line and prepare for processing
input_data = [line.strip() for line in lines]

# Define symbol names and their corresponding line counters
symbol_names = ['A', 'L1', 'B', 'C', 'D', 'L2']
symbol_LC = [100, 103, 108, 99, 101, 106]

# Literal table data
literal_names = ['="5"', 'L2']
literal_values = [500, 600]

def convert_input(intercode_data):
    global result
    # Converts input data into structured format for processing
    for line in intercode_data:
        line = line.strip()
        if line:
            parts = line.split()
            if not parts[0].startswith('('):
                num = int(parts[0])
                inner_parts = [part.strip('()').split(',') for part in parts[1:]]
                print("Printing inner part")
                print(inner_parts)
                result.append([num] + inner_parts)
    print("Converted Input:", result)

def find_symbol_value(index):
    # Get symbol value from symbol_LC based on index
    return symbol_LC[index - 1]

def find_literal_value(index):
    # Get literal value and name from literal_names and literal_values lists
    return literal_names[index - 1], literal_values[index - 1]

def pass2(input_data):
    global output
    convert_input(input_data)
    for item in result:
        print("Item")
        print(item)
        oline = ""
        for i in range(len(item)):
            print("i")
            print(i)
            element =item[i]
            if isinstance(element, int):
                oline += str(element)
            else:
                # Processing based on type of element
                if element[0] in ("IS", "CC", "R", "C"):
                    oline += f" {element[1]} "
                elif element[0] == "L":
                    # Look up literal name and value
                    name, value = find_literal_value(int(element[1]))
                    oline += f" {name}" if i == 1 else f" {value}"
                elif element[0] == "S":
                    # Look up symbol value
                    value = find_symbol_value(int(element[1]))
                    oline += f" {value}"
                elif element[0] == "DL":
                    # Processing directives (DL)
                    if element[1] == '1':
                        oline += f" {item[2][1]}"
                        break
                    elif element[1] == '2':
                        # Handle storage directive with line counting
                        lc = int(item[0])
                        oline = ""
                        print("int(item[2][1])",int(item[2][1]))
                        for _ in range(int(item[2][1])):
                            oline += f"{lc} \n"
                            print("oline",oline)
                            lc += 1
                        break

        output.append(f"\n {oline}")

# Run pass2 with input data
pass2(input_data)
print("Output:", output)

file = open("output.txt",'w')

file.writelines(output)