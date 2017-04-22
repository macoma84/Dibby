package com.dibby;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Date;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/")
public class DibbyRestService {
	@POST
	@Path("/dibbyService")
	@Consumes(MediaType.APPLICATION_JSON)
	public Response dibbyREST(InputStream incomingData) {
		StringBuilder dibbyBuffer = new StringBuilder();
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(incomingData));
			String line = null;
			while ((line = in.readLine()) != null) {
				dibbyBuffer.append(line);
			}
		} catch (Exception e) {
			System.out.println("Error Parsing: - ");
		}
		System.out.println("Data Received: " + dibbyBuffer.toString());
 
		// return HTTP response 200 in case of success
		return Response.status(200).entity(dibbyBuffer.toString()).build();
	}
	
	
	
	@POST
	@Path("/dibbyDB")
	@Consumes(MediaType.APPLICATION_JSON)
	public Response dibbyRESTDB(String param1) { 
	 
		System.out.println("Data Received: " + param1);
		String result = "OK";
 
		// return HTTP response 200 in case of success
		return Response.status(200).entity(result).build();
	}
 
	@GET
	@Path("/verify")
	@Produces(MediaType.TEXT_PLAIN)
	public Response verifyRESTService(@QueryParam("db")  @DefaultValue("") String db ,@QueryParam("hz")  @DefaultValue("") String hz ,@QueryParam("env")  @DefaultValue("") String env ) {
		
		Date date = new Date();
		
		System.out.println("Data Received|" + db + "|" + hz +"|"+ env + "|" + date.getTime());
		
		
		String result = "OK";
 
		// return HTTP response 200 in case of success
		return Response.status(200).entity(result).build();
	}
 
}