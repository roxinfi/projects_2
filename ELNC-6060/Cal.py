# math_operations.py

def add(a, b):
    """Add two numbers"""
    return a + b

def subtract(a, b):
    """Subtract two numbers"""
    return a - b

def multiply(a, b):
    """Multiply two numbers"""
    return a * b

def divide(a, b):
    """Divide two numbers"""
    if b != 0:
        return a / b
    else:
        raise ValueError("Cannot divide by zero")

# Additional math operations can be added here