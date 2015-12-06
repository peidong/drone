#gps_ubidots part

1. HTTP GET (Not recommend)

        http://fryer.ee.ucla.edu/rest/api/gps_ubidots/get/?location_type=0

2. HTTP POST

    Post data example

       face_direction=120&latitude=34.0&longitude=118.0

    Url

        http://fryer.ee.ucla.edu/rest/api/gps_ubidots/post/

###Database part

Database: edison, table: gps_ubidots

#####face_direction(int 11)

- The direction degree

#####latitude(double 9,6)

- The latitude degree

#####longitude(double 9,6)

- The longitude degree
