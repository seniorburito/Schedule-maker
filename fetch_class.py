from bs4 import BeautifulSoup
from urllib.request import urlopen
import csv, json

url = "https://stolaf.dev/course-data/terms/20161.json"
# url = "https://www.stolaf.edu/sis/static-classlab/20183.html"
html = urlopen(url).read()
# soup = BeautifulSoup(html, features="lxml")
# table = soup.find_all('table')[0]
emp_data = json.loads(html)

with open("out.csv", "w") as f:
    wr = csv.writer(f)

    count = 0

    for emp in emp_data:
        if count == 0:

            header = emp.keys()

            wr.writerow(header)

            count += 1

        wr.writerow(emp.values())


    # # read each line of html file
    # row_marker = 0
    # for row in table.find_all('tr'):
    #     row_csv = []
    #     column_marker = 0
    #     columns = row.find_all('td')
    #     for column in columns:
    #         row_csv.append(column.get_text)
    #     wr.writerows(row)









