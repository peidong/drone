<?php
    $uploadpath = "files/";
    $filedata = $_FILES['file']['tmp_name'];
    echo "filedata= ".$filedata;
    if ($filedata != ''){
        copy($filedata,$uploadpath."1.c");
        echo "success";
    }
?>
