from sys import argv as args
args = args[1:]



class JSObject:
    def __init__(self,args={}):
        self.__dict__ = args 
    def __getattribute__(self, key ) :
        try:
            return object.__getattribute__(self, key)
        except :
            return None
        

def main():
    pass


if __name__ == "__main__":
    main()

