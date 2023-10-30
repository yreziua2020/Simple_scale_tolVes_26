void bd_sql() {


if (!client.connect(DB_host,Port)) {
    Serial.print(DB_host);
    Serial.println(" - Connection failed.");
    client.stop();
    f_eror_sql=1;
  }
    if (client.connect(DB_host, Port))
    {
    Serial.println("database connection established");  
    client.print( "GET /php/wp-scale2.php?A=Vp3wDWbP&ID=");
    client.print(UserID);
    client.print("&T1=");
    client.print(units); //data
    client.print("&P=");
    client.print(database_pass);
    client.println( " HTTP/1.1");
    client.print( "Host:" );
    client.println(DB_host);
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
    
    }


  
}
