LC = 0

symbol_names = []
symbol_Lc = []
literal_name =[]
literal_Lc= []

input_data = []
with open ('input.txt','r') as file :
    input_data = file.readlines()
print(input_data)
input_data = [line.strip() for line in input_data]
print(input_data)

class OPTAB:
    @staticmethod
    def is_IS(opcode):
        instructions = ['STOP','ADD','SUB' , 'MULT', 'MOVER','MOVEM','COMP','BC','DIV','READ','PRINT']
        return instructions.index(opcode) if opcode in instructions else -1 
    
    @staticmethod
    def is_AD(opcode):
        instructions = ['START','END','ORIGIN','EQU','LTORG']
        return instructions.index(opcode) if opcode in instructions else -1
    
    @staticmethod
    def is_REG(opcode):
        instructions = ['AREG','BREG','CREG','DREG']
        return instructions.index(opcode)+1 if opcode in instructions else -1
    
    @staticmethod
    def is_DC_DS(opcode):
        return opcode if opcode in ["DS","DC"] else -1
    
    @staticmethod
    def is_Constant(opcode):
        return ( OPTAB.is_IS(opcode) == -1 and OPTAB.is_AD(opcode) == -1 and OPTAB.is_REG(opcode) == -1 and OPTAB.is_DC_DS(opcode) == -1)
    
class Process:

    @staticmethod
    def for_start(ins):
        global LC
        LC = int(ins[1])
        line_op = f"(AD,01) (C,{LC})"
        return line_op

    @staticmethod
    def for_DS(ins):
        # Process the DS directive: defines storage and updates the symbol table
        global LC
        if ins[0] not in symbol_names:
            symbol_names.append(ins[0])
            symbol_Lc.append(-1)
        index = symbol_names.index(ins[0])
        symbol_Lc[index] = LC

        storage = int(ins[2])
        line_output = f"{LC:03} (DL,2) (C,{storage})"
        LC += storage

        return line_output
    
    @staticmethod
    def for_DC(ins):
        # Process the DC directive: defines a constant and updates the symbol table
        global LC
        index = symbol_names.index(ins[0])
        symbol_Lc[index] = LC

        value_str = ins[2].strip("'")
        const_value = int(value_str)

        line_output = f"{LC:03} (DL,1) (C,{const_value})"
        LC += 1
        return line_output
    
    @staticmethod
    def for_ltorg(ins):
        # Process the LTORG directive: assigns addresses to literals
        global LC
        oline = ""
        for i in range(len(literal_Lc)):
            if literal_Lc[i] == -1:
                oline += f"{LC} (L, {i:02}) \n"
                literal_Lc[i] = LC
                LC += 1
        return (oline)
    
    @staticmethod
    def for_origin(ins):
        # Process the ORIGIN directive: sets LC based on symbol's value
        global LC
        oline = ""
        num = int(ins[3])
        sym = ins[1]
        index = symbol_names.index(sym)
        print(index)
        LC_value = symbol_Lc[index]
        print(LC_value)
        oline = f"{LC:03} ( AD, 03)"
        LC = LC_value + num
        return oline

    @staticmethod
    def for_equ(ins):
        # Process the EQU directive: defines symbols with specific values
        symb1 = ins[0]
        symb2_equation = ins[2]
        index = symbol_names.index(symb2_equation)
        print(index)
        LC_value = symbol_Lc[index]
        symbol_names.append(symb1)
        symbol_Lc.append(LC_value)
        oline = f"(AD,03)"
        return oline



def main():
    global LC 
    output = []

    for instruction in input_data:
        instruction = instruction.split()
        print(instruction)
        line_output = ""

        if OPTAB.is_AD(instruction[0]) == -1:
            line_output = f"{LC:03}"
        else:
            line_output = " "
        
        if instruction[0] == "START":
            start_line =Process.for_start(instruction)
            output.append(start_line)
            continue

        if "DS" in instruction :
            ds_line = Process.for_DS(instruction)
            output.append(ds_line)
            continue

        if "DC" in instruction:
            dc_line = Process.for_DC(instruction)
            output.append(dc_line)
            continue
                # Check if the instruction has "EQU"
        if "EQU" in instruction:
            oline = Process.for_equ(instruction)
            output.append(oline)
            continue

        # Check if the instruction has "ORIGIN"
        if "ORIGIN" in instruction:
            o=Process.for_origin(instruction)
            output.append(o)
            continue

        # Check if the instruction has "LTORG"
        if "LTORG" in instruction or "END" in instruction:
            lit = Process.for_ltorg(instruction)
            output.append(lit)

        for token in instruction:
            if OPTAB.is_Constant(token):
                if "=" in token:
                    literal_name.append(token)
                    literal_Lc.append(-1)
                    indexT = literal_name.index(token)
                    line_output += f"(L,{indexT:02})"
                    continue

                if instruction[0] == token and token not in symbol_names:
                    symbol_names.append(token)
                    symbol_Lc.append(LC)

                elif token not in symbol_names :
                    symbol_names.append(token)
                    symbol_Lc.append(-1)
                    line_output += f"(S,{symbol_names.index(token):02})"

                else:
                    line_output += f"(S,{symbol_names.index(token):02})"

            elif OPTAB.is_IS(token) != -1:
                num = OPTAB.is_IS(token)
                line_output += f"(IS,{num:02}) "
                LC += 1

            elif OPTAB.is_REG(token) != -1:
                num = OPTAB.is_REG(token)
                line_output += f"({num}) "

        output.append(line_output.strip()) 

    with open('output.txt', 'w') as file:
        for line in output:
            file.write(line + "\n")

    print(symbol_names)
    print(symbol_Lc)
    print(literal_name)
    print(literal_Lc)
        


if __name__ == "__main__":
    main()
