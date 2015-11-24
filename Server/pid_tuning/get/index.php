<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    $query = "SELECT *
        FROM pid_tuning
        WHERE id=1";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $kp_pitch = $result_array[kp_pitch];
    $ki_pitch = $result_array[ki_pitch];
    $kd_pitch = $result_array[kd_pitch];
    $kp_roll = $result_array[kp_roll];
    $ki_roll = $result_array[ki_roll];
    $kd_roll = $result_array[kd_roll];
    $kp_yaw = $result_array[kp_yaw];
    $ki_yaw = $result_array[ki_yaw];
    $kd_yaw = $result_array[kd_yaw];
    $kp_second_pitch = $result_array[kp_second_pitch];
    $kd_second_pitch = $result_array[kd_second_pitch];
    $kp_second_roll = $result_array[kp_second_roll];
    $kd_second_roll = $result_array[kd_second_roll];
    $kp_second_yaw = $result_array[kp_second_yaw];
    $kd_second_yaw = $result_array[kd_second_yaw];
    $update_time = $result_array[update_time];

    $response['kp_pitch'] = $kp_pitch;
    $response['ki_pitch'] = $ki_pitch;
    $response['kd_pitch'] = $kd_pitch;
    $response['kp_roll'] = $kp_roll;
    $response['ki_roll'] = $ki_roll;
    $response['kd_roll'] = $kd_roll;
    $response['kp_yaw'] = $kp_yaw;
    $response['ki_yaw'] = $ki_yaw;
    $response['kd_yaw'] = $kd_yaw;
    $response['kp_second_pitch'] = $kp_second_pitch;
    $response['kd_second_pitch'] = $kd_second_pitch;
    $response['kp_second_roll'] = $kp_second_roll;
    $response['kd_second_roll'] = $kd_second_roll;
    $response['kp_second_yaw'] = $kp_second_yaw;
    $response['kd_second_yaw'] = $kd_second_yaw;
    $response['update_time'] = $update_time;

    deliver_response(200, "The pid_tuning value has been got", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
