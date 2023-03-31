from pyspark import SparkContext,SparkConf
import pyspark.sql.functions as ssql
from pyspark.sql.types import *
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.sql.functions import *
import sys

from pyspark.sql import SQLContext
 

def check(processors, file_with_path):


    if processors <= 0:
        print("less number of  procesors")
        exit()


    if file_with_path == "":
        print("file not exist ")



file_with_path = sys.argv[2]
processors = int(sys.argv[1])
check(processors, file_with_path)
getting_spark = SparkSession.builder.getOrCreate()
getting_dataframe = getting_spark.read.csv('airports.csv', inferSchema=True, header=True)
getting_dataframe = getting_dataframe.repartition(processors)
check(processors, file_with_path)
end_data = getting_dataframe.groupBy('COUNTRY').agg(ssql.count('COUNTRY').alias('COUNT'))
end_data.toPandas().to_csv(file_with_path)	
