import sys
from pyspark.sql import SparkSession
import os
import glob

spark = SparkSession.builder.master("local").appName('lab5').getOrCreate()

if(len(sys.argv) != 3):
    print("Invalid arguments..")
    sys.exit(1)
opname = sys.argv[2]
cpu = int(sys.argv[1])

df = spark.read.csv('Dataset/airports.csv', inferSchema=True, header=True)
df_new = df.repartition(cpu)

df_new.select("Name").filter("(Latitude between 10 and 90) and (Longitude between -90 and -10)").coalesce(1).write.format("com.databricks.spark.csv").option("header", True).save(opname+"op")
for file in glob.glob("{}op/*.csv".format(opname)):
    os.rename(file,opname + ".csv")
import shutil
shutil.rmtree("{}op".format(opname), ignore_errors=True)