import matplotlib.pyplot as plt
import numpy as np


def line(x, y, title="Data Plot", xlabel="X-axis", ylabel="Y-axis"):
    """
    Plots the given x and y data using matplotlib.

    Parameters:
    x (list or np.array): The x data points.
    y (list or np.array): The y data points.
    title (str): The title of the plot.
    xlabel (str): The label for the x-axis.
    ylabel (str): The label for the y-axis.
    """
    plt.figure(figsize=(10, 6))
    plt.plot(x, y, marker='o')
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.show()


def scatter(x, y, title="Scatter Plot", xlabel="X-axis", ylabel="Y-axis"):
    """
    Plots a scatter plot of the given x and y data using matplotlib.

    Parameters:
    x (list or np.array): The x data points.
    y (list or np.array): The y data points.
    title (str): The title of the plot.
    xlabel (str): The label for the x-axis.
    ylabel (str): The label for the y-axis.
    """
    plt.figure(figsize=(10, 6))
    plt.scatter(x, y, c='blue', alpha=0.5)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.show()


def histogram(data, bins=10, title="Histogram", xlabel="Value", ylabel="Frequency"):
    """
    Plots a histogram of the given data using matplotlib.

    Parameters:
    data (list or np.array): The data points to plot.
    bins (int): The number of bins for the histogram.
    title (str): The title of the plot.
    xlabel (str): The label for the x-axis.
    ylabel (str): The label for the y-axis.
    """
    plt.figure(figsize=(10, 6))
    plt.hist(data, bins=bins, edgecolor='black')
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.show()

def heatmap(data, title="Heatmap", xlabel="X-axis", ylabel="Y-axis"):
    """
    Plots a heatmap of the given 2D data using matplotlib.

    Parameters:
    data (2D list or np.array): The 2D data points to plot.
    title (str): The title of the plot.
    xlabel (str): The label for the x-axis.
    ylabel (str): The label for the y-axis.
    """
    plt.figure(figsize=(10, 6))
    plt.imshow(data, cmap='hot', interpolation='nearest')
    plt.colorbar()
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.show()


