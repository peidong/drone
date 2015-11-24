<?php
    header("Content-Type:application/json");

    // start the session
    session_start();

    $kp_pitch = $_POST['kp_pitch'];
    $ki_pitch = $_POST['ki_pitch'];
    $kd_pitch = $_POST['kd_pitch'];
    $kp_roll = $_POST['kp_roll'];
    $ki_roll = $_POST['ki_roll'];
    $kd_roll = $_POST['kd_roll'];
    $kp_yaw = $_POST['kp_yaw'];
    $ki_yaw = $_POST['ki_yaw'];
    $kd_yaw = $_POST['kd_yaw'];
    $kp_second_pitch = $_POST['kp_second_pitch'];
    $kd_second_pitch = $_POST['kd_second_pitch'];
    $kp_second_roll = $_POST['kp_second_roll'];
    $kd_second_roll = $_POST['kd_second_roll'];
    $kp_second_yaw = $_POST['kp_second_yaw'];
    $kd_second_yaw = $_POST['kd_second_yaw'];

    // close the session
    session_write_close();

    $conn = mysql_connect('localhost', 'webmaster', '');
    mysql_select_db('edison', $conn);

    $query = "SELECT *
        FROM pid_tuning
        WHERE id = '1'";

    $result = mysql_query($query);
    $result_array = mysql_fetch_array($result);

    $last_time_kp_pitch = $result_array[kp_pitch];
    $last_time_ki_pitch = $result_array[ki_pitch];
    $last_time_kd_pitch = $result_array[kd_pitch];
    $last_time_kp_roll = $result_array[kp_roll];
    $last_time_ki_roll = $result_array[ki_roll];
    $last_time_kd_roll = $result_array[kd_roll];
    $last_time_kp_yaw = $result_array[kp_yaw];
    $last_time_ki_yaw = $result_array[ki_yaw];
    $last_time_kd_yaw = $result_array[kd_yaw];
    $last_time_kp_second_pitch = $result_array[kp_second_pitch];
    $last_time_kd_second_pitch = $result_array[kd_second_pitch];
    $last_time_kp_second_roll = $result_array[kp_second_roll];
    $last_time_kd_second_roll = $result_array[kd_second_roll];
    $last_time_kp_second_yaw = $result_array[kp_second_yaw];
    $last_time_kd_second_yaw = $result_array[kd_second_yaw];
    $last_time_update_time = $result_array[update_time];

    if($kp_pitch == NULL){
        $kp_pitch = $last_time_kp_pitch;
    }
    if($ki_pitch == NULL){
        $ki_pitch = $last_time_ki_pitch;
    }
    if($kd_pitch == NULL){
        $kd_pitch = $last_time_kd_pitch;
    }
    if($kp_roll == NULL){
        $kp_roll = $last_time_kp_roll;
    }
    if($ki_roll == NULL){
        $ki_roll = $last_time_ki_roll;
    }
    if($kd_roll == NULL){
        $kd_roll = $last_time_kd_roll;
    }
    if($kp_yaw == NULL){
        $kp_yaw = $last_time_kp_yaw;
    }
    if($ki_yaw == NULL){
        $ki_yaw = $last_time_ki_yaw;
    }
    if($kd_yaw == NULL){
        $kd_yaw = $last_time_kd_yaw;
    }
    if($kp_second_pitch == NULL){
        $kp_second_pitch = $last_time_kp_second_pitch;
    }
    if($kd_second_pitch == NULL){
        $kd_second_pitch = $last_time_kd_second_pitch;
    }
    if($kp_second_roll == NULL){
        $kp_second_roll = $last_time_kp_second_roll;
    }
    if($kd_second_roll == NULL){
        $kd_second_roll = $last_time_kd_second_roll;
    }
    if($kp_second_yaw == NULL){
        $kp_second_yaw = $last_time_kp_second_yaw;
    }
    if($kd_second_yaw == NULL){
        $kd_second_yaw = $last_time_kd_second_yaw;
    }

    $query = "UPDATE pid_tuning
        SET kp_pitch = '$kp_pitch', ki_pitch = '$ki_pitch', kd_pitch = '$kd_pitch', kp_roll = '$kp_roll', ki_roll = '$ki_roll', kd_roll = '$kd_roll', kp_yaw = '$kp_yaw', ki_yaw = '$ki_yaw', kd_yaw = '$kd_yaw', kp_second_pitch = '$kp_second_pitch', kd_second_pitch = '$kd_second_pitch', kp_second_roll = '$kp_second_roll', kd_second_roll = '$kd_second_roll', kp_second_yaw = '$kp_second_yaw', kd_second_yaw = '$kd_second_yaw', update_time = now()
        WHERE id = '1'";

    $result = mysql_query($query);

    $query = "SELECT *
        FROM pid_tuning
        WHERE id = '1'";

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

    deliver_response(200, "The pid_tuning commands have been updated", $response);

    function deliver_response($status,$status_message,$data){
        header("HTTP/1.1 $status $status_message");
        $response['status']=$status;
        $response['status_message']=$status_message;
        $response['data']=$data;

        $json_response=json_encode($response);
        echo $json_response;
    }
?>
