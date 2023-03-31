import sys
from pyspark.sql import SparkSession
from pyspark.sql.functions import col


spark = SparkSession.builder.master("local").appName('lab5').getOrCreate()

if(len(sys.argv) != 3):
    print("Invalid arguments..")
    sys.exit(1)
opname = sys.argv[2]
cpu = int(sys.argv[1])

df = spark.read.csv('Dataset/airports.csv', inferSchema=True, header=True)
df_new = df.repartition(cpu)

df_new.groupBy("Country").count().selectExpr("Country","count as Airports").sort(col("count").desc()).limit(1).toPandas().to_csv(opname + ".csv",index = False)