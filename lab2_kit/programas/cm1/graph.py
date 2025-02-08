import matplotlib.pyplot as plt
import pandas as pd
import numpy as np  # Import numpy for log scale

# Function to read the input file and parse it into a DataFrame
def read_file(filename):
    data = []
    
    with open(filename, 'r') as file:
        for line in file:
            parts = line.split()
            cache_size = int(parts[0].split('=')[1])
            stride = int(parts[1].split('=')[1])
            avg_misses = float(parts[2].split('=')[1])
            avg_time = float(parts[3].split('=')[1])
            data.append([cache_size, stride, avg_misses, avg_time])
    
    # Convert the data into a DataFrame for easier plotting
    df = pd.DataFrame(data, columns=['cache_size', 'stride', 'avg_misses', 'avg_time'])
    return df

# Function to plot the graph and save it as a PNG
def plot_graph(df, output_filename):
    plt.figure(figsize=(10, 6))

    # Group the data by cache_size and plot for each group
    for cache_size, group in df.groupby('cache_size'):
        plt.plot(group['stride'], group['avg_misses'], label=f'{cache_size} cache_size', marker='o')

    # Setting labels and title
    plt.xlabel('Stride')
    plt.ylabel('Average Misses')
    plt.title('Average Misses vs Stride for Different Cache Sizes')
    plt.legend(title='Cache Size')
    plt.grid(True, which="both", ls="--")  # Ensure grid for both major and minor ticks

    #Set log scale for x-axis and define ticks for powers of 2 from 2^0 to 2^13
    plt.xscale('log', base=2)
    #plt.xticks([2**i for i in range(20)], [f'2^{i}' for i in range(20)])

    # Ensure y-axis starts at 0
    plt.ylim(bottom=0)

    # Save the plot as a PNG file
    plt.savefig(output_filename)
    plt.close()  # Close the plot to free up memory

# Main function to read the file and generate the plot
def main():
    # Replace 'input.txt' with your actual file name
    filename = 'data.txt'
    output_filename = 'output_plot.png'
    
    df = read_file(filename)
    plot_graph(df, output_filename)

    print(f"Plot saved as {output_filename}")

if __name__ == '__main__':
    main()
