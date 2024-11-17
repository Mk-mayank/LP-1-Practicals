file = open(r"input.txt", "r")
lines = file.readlines()
input_file = [line.strip() for line in lines]


class Process:
    MNT = {}
    MDT = {}
    KPDTAB = {}
    PNTAB = []

    MNT_counter = 1
    KPDTAB_counter = 0
    MDTP_counter = 1

    got_header = False

    @staticmethod
    def main(input_file):
        for instruction in input_file:
            if instruction == "MACRO":
                Process.got_header = True

            elif instruction == "MEND":
                Process.process_mend()

            elif Process.got_header == True:
                Process.got_header = False
                Process.process_mnt(instruction)
            else:
                Process.process_body(instruction)

    @staticmethod
    def process_mnt(ins):
        definition = ins.split()
        name, parameters = definition[0], definition[1].split(',')
        print(name, parameters)

        current_kpdtp_index = Process.KPDTAB_counter + 1
        keyword = 0
        positional = 0

        for i in parameters:
            print("Printing --------------------")
            print(i)
            if "=" in i:
                keyword += 1
                Process.PNTAB.append(i.split("=")[0])
                print(i.split("=")[0])# Append the parameter name, not the whole key-value pair
                Process.process_kpdtab(i)
            else:
                positional += 1
                Process.PNTAB.append(i)

        kpdtp = "-" if keyword == 0 else current_kpdtp_index

        Process.MNT[Process.MNT_counter] = {
            'Name': f'{name}',
            '#PP': f"{positional}",
            '#KP': f"{keyword}",
            'MDTP': f"{Process.MDTP_counter}",
            'KPDTP': f"{kpdtp}"
        }
        Process.MNT_counter += 1

    @staticmethod
    def process_kpdtab(parameter):
        parameter = parameter.split("=")
        value = "No default value"
        if parameter[1] != "":
            value = parameter[1]

        Process.KPDTAB[Process.KPDTAB_counter + 1] = {
            'Name': f'{parameter[0]}',
            'Value': f'{value}'
        }
        Process.KPDTAB_counter += 1

    @staticmethod
    def process_body(word):
        instruction = word.split()[0]
        print("Instruction in body mdt:")
        print(instruction)
        params = word.split()[1].split(',')
        print("Params in body mdt:")
        print(params)
        ins_line = instruction + " "
        param_references = []

        print("PNTAB:", Process.PNTAB)  # Debugging line to check PNTAB content

        for param in params:
            if param not in Process.PNTAB:
                print(f"Error: Parameter {param} not found in PNTAB")
                continue  # Skip this parameter and continue processing

            index = Process.PNTAB.index(param)  # Access the index of the parameter if it exists

            if index > 3:
                index -= 3

            param_references.append(f"(P,{index})")
            print("print param_references:")
            print(param_references)

        ins_line += " ".join(param_references)
        print("ins_line:")
        print(ins_line)
        Process.MDT[Process.MDTP_counter] = ins_line
        Process.MDTP_counter += 1

    @staticmethod
    def process_mend():
        Process.MDT[Process.MDTP_counter] = "MEND"
        Process.MDTP_counter += 1

    @staticmethod
    def show_Result():
        counter = 0
        print("Sr.No\tName \t#PP\t#KP\tMDTP\tKPDTP")
        for entry in Process.MNT.values():
            counter += 1
            print(counter, end="\t")
            print(entry['Name'], end=" \t")
            print(entry['#PP'], end="\t")
            print(entry['#KP'], end="\t")
            print(entry['MDTP'], end="\t")
            print(entry['KPDTP'], end="\n")

        print("\nMDT")
        print("Sr.No\tInstruction")
        print(Process.MDT.items())
        for sr_no, instruction in Process.MDT.items():
            print(f"{sr_no}\t{instruction}")

        print("\nKPDTAB")
        print("Sr.No\tName\t\tValue")
        print("Printing KPDTAB:")
        print(Process.KPDTAB.items())
        for sr_no, entry in Process.KPDTAB.items():
            print(f"{sr_no}\t{entry['Name']:10}\t{entry['Value']}")


def start():
    Process.main(input_file)
    Process.show_Result()
    file.close()


start()