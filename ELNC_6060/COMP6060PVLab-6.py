print("Welcome to VRAJ's statistical analysis program")

# creating a dataset
dataset = [56,74,-10,58,4,17,26,0,13,37]

# printing the dataset_size
dataset_size = len(dataset)
print(f'Dataset size has {dataset_size} elements')

# removing negative values from the dataset
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
mean = int(dataset_sum / dataset_size)
print(f'The mean of the dataser = {mean}')

# calculating the median of the dataset
if dataset_size % 2 == 0:   # if the dataset size is even
    mid_value_1 = dataset[dataset_size//2]
    mid_value_2 = dataset[(dataset_size//2) + 1]
    median = int((mid_value_1 + mid_value_2) / 2)
    print(f'The median of the dataset = {median}')
else:                       # if the dataset size is odd
    median = int(dataset[dataset_size//2])
    print(f'The median of the dataset = {median}')
