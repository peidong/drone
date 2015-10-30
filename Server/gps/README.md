#gps part

1. HTTP GET

        http://fryer.ee.ucla.edu/rest/api/gps/get/?location_type=0

2. HTTP POST

    Post data example

       face_direction=120.0&latitude=34.0&longitude=118.0 

    Url
    
        http://fryer.ee.ucla.edu/rest/api/gps/post/?location_type=0

###Database part

Database: edison, table: gps

#####location_type
- 0: Destination
- 1: Drone's location

#####face_direction

- The direction degree

#####latitude

- The latitude degree

#####longitude

- The longitude degree
