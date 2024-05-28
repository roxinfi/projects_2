print("Welcome to VRAJ's statistical analysis program")

# creating a dataset
dataset = [56,74,-10,58,4,17,26,0,13,37]

# printing the dataset
dataset_size = len(dataset)
print(f'Dataset size has {dataset_size} elements')

# removing negative values
for i in range(dataset_size):
    if dataset[i] < 0:
        dataset[i] = 0

#sorting the dataset
dataset.sort()

#calculating the sum of the dataset
dataset_sum = 0
for val in dataset:
    dataset_sum += val


# calculating the mean of the dataset
mean = float(dataset_sum / dataset_size)
print(f'The mean of the dataser = {mean}')