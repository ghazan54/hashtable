import matplotlib.pyplot as plt


def draw(data_files, labels, title, output_file):
    fig, ax = plt.subplots()

    for data_file, label in zip(data_files, labels):
        x = []
        y = []
        with open(data_file, 'r') as file:
            for line in file:
                values = line.strip().split()
                x.append(float(values[0]))
                y.append(float(values[1]))

        ax.plot(x, y, label=label)

    ax.legend()

    ax.set_title(title)
    ax.set_xlabel('Количество элементов')
    ax.set_ylabel('Время, с')

    # Изменение оси X
    ax.set_xticks(x)
    ax.set_xticklabels([str(int(value)) for value in x])

    dpi = 300

    plt.savefig(output_file, dpi=dpi)


if __name__ == "__main__":
    draw(["result/time/djb2.dat", "result/time/elfHash.dat", "result/time/fnvHash.dat", "result/time/jen.dat", "result/time/krHash.dat"],
         ["djb2", "elfHash", "fnvHash", "jen", "krHash"], "", "time.png")
    # draw(["result/col/djb2.dat", "result/col/elfHash.dat", "result/col/fnvHash.dat", "result/col/jen.dat", "result/col/krHash.dat"],
    #      ["djb2", "elfHash", "fnvHash", "jen", "krHash"], "", "col.png")
