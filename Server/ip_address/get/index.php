<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);
    
    for ($i=1; $i<=5; $i++)
    {
        $query = "SELECT id, ip_address, mac_address, network_name, update_time
            FROM ip_address
            WHERE id='$i'";

        $result = mysql_query($query);
        $result_array = mysql_fetch_array($result);

        $ip_address = $result_array[ip_address];
        $mac_address = $result_array[mac_address];
        $network_name = $result_array[network_name];
        $update_time = $result_array[update_time];

        $response['ip_address'] = $ip_address;
        $response['mac_address'] = $mac_address;
        $response['network_name'] = $network_name;
        $response['update_time'] = $update_time;

        $response_array[$i] = $response;
    }
    deliver_response(200, "The ip address has been got", $response_array);
    
    /*
    $query = "SELECT id, ip_address, mac_address, network_name, update_time
        FROM ip_address
        WHERE id='1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $ip_address = $result_array[ip_address];
    $mac_address = $result_array[mac_address];
    $network_name = $result_array[network_name];
    $update_time = $result_array[update_time];

    $response['ip_address'] = $ip_address;
    $response['mac_address'] = $mac_address;
    $response['network_name'] = $network_name;
    $response['update_time'] = $update_time;

    deliver_response(200, "The ip address has been got", $response);
     */

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
