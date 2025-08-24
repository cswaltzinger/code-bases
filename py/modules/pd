import pandas as pd
import numpy as np
import mysql


def read_csv(file_path):
    return pd.read_csv(file_path)


def get_from_dict(data_dict):
    return pd.DataFrame.from_dict(data_dict)


def make_sql_connection(host, user, password, database):
    
    return mysql.connector.connect(
        host=host,
        user=user,
        password=password,
        database=database
    )


def get_from_sql(query, connection):
    return pd.read_sql(query, connection)

def to_numpy(df):
    return df.to_numpy()

def df_columns(df):
    return df.columns.tolist()

def iterate_rows(df):
    for index, row in df.iterrows():
        print(index, row)

def filter_rows(df, condition):
    return df[condition]

def iloc_example(df, row_idx, col_idx):
    return df.iloc[row_idx, col_idx]

def loc_example(df, row_label, col_label):
    return df.loc[row_label, col_label]

def group_by_example(df, col_name):
    return df.groupby(col_name).size()

def describe_df(df):
    return df.describe()

def fill_na(df, value):
    return df.fillna(value)

