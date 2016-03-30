# import the necessary packages
from imagespider.items import ImagespiderItem
import datetime
import scrapy
import sys

class Spider_(scrapy.Spider):
	name = "pyimagesearch-spider"
	start_urls = ['https://www.bing.com/images/search?q=goat&go=Submit&qs=n&form=QBILPG&pq=goat&sc=8-4&sp=-1&sk=']

	def parse(self, response):
                # let's only gather Time U.S. magazine covers
                url = response.xpath("//img")
                imgURL = url.extract()
                for i, val in enumerate(imgURL):
                    yield ImagespiderItem(name ="goat%i" %i, file_urls=[val])
                    #imageURL = url.extract()
                    #yield ImagespiderItem(file_urls=[imageURL])
