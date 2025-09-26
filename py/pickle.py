import pickle

def get(path):
    with open(path, 'rb') as file:
        return pickle.load(file)
    return None

def save(path, obj):
    with open(path, 'wb') as file:
        pickle.dump(obj, file)  