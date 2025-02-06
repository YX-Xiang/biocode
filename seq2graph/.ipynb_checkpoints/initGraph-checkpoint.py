import random

def generate_dna_sequence(length=1000):
    # 生成指定长度的随机DNA序列
    return ''.join(random.choices("ACGT", k=length))

def edit_dna_sequence(sequence, edit_count):
    # 对DNA序列进行随机编辑操作（插入、删除、替换）
    seq = list(sequence)
    for _ in range(edit_count):
        operation = random.choice(["insert", "delete", "replace"])
        pos = random.randint(0, len(seq) - 1)
        if operation == "insert":
            seq.insert(pos, random.choice("ACGT"))
        elif operation == "delete" and len(seq) > 1:
            seq.pop(pos)
        elif operation == "replace":
            seq[pos] = random.choice("ACGT")
    return ''.join(seq)

def generate_sequences(output_file):
    # 生成原始DNA序列及9条编辑后的序列，并写入文件
    original_sequence = generate_dna_sequence()
    sequences = [original_sequence] + [edit_dna_sequence(original_sequence, random.randint(1, 10)) for _ in range(9)]
    
    with open(output_file, "w") as f:
        for i, seq in enumerate(sequences):
            f.write(f">Sequence_{i+1}\n{seq}\n")
    print(f"Sequences saved to {output_file}")

# 指定输出文件名
generate_sequences("graph/seq_10.fasta")
