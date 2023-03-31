from bs4 import BeautifulSoup as soup
from urllib.request import urlopen
import csv

home = "https://www.amazon.in"
my_url = ["https://www.amazon.in/gp/bestsellers/books/ref=zg_bs_pg_1?ie=UTF8&pg=1","https://www.amazon.in/gp/bestsellers/books/ref=zg_bs_pg_2/260-8744875-9347404?ie=UTF8&pg=2"]
url_client = urlopen(my_url[0])
page_html1 = url_client.read()
url_client.close()
url_client = urlopen(my_url[1])
page_html2 = url_client.read()
url_client.close()

page_soup1 = soup(page_html1, "html.parser")
page_soup2 = soup(page_html2, "html.parser")
containers = []

containers.append(page_soup1.findAll("div",{"class": "a-section a-spacing-none aok-relative"}))
containers.append(page_soup2.findAll("div",{"class": "a-section a-spacing-none aok-relative"}))
items=[]
for con in containers:
	for data in con:
		try:
			link = home + data.find("a",{"class": "a-link-normal"})["href"]
		except AttributeError:
			link = "Not Available"
		try:
			author = data.find("a",{"class":"a-size-small a-link-child"}).text.strip()
		except AttributeError:
			author = "Not Available"
		try:
			title = data.find("div",{"class": "p13n-sc-truncate p13n-sc-line-clamp-1 p13n-sc-truncate-desktop-type2"}).text.strip()
		except AttributeError:
			title = "Not Available"
		try:
			rating = data.find("span",{"class":"a-icon-alt"}).text.strip()
		except AttributeError:
			rating = "Not Available"
		try:
			price = data.find("span",{"class":"p13n-sc-price"}).text.strip()
		except AttributeError:
			price = "Not Available"
		try:
			n_ratings = data.find("a",{"class":"a-size-small a-link-normal"}).text.strip()
		except AttributeError:
			n_ratings = "Not Available"
	
		items.append([title,link,author,price,n_ratings,rating])


field = ["Name","URL","Author","Price","Number of Ratings","Average Ratings"]

with open("output/in_book.csv","w") as f:
	write = csv.writer(f,delimiter=";")
	write.writerow(field)
	write.writerows(items)
