// Import the Express module
import express from "express";

// Import the CORS module
import cors from "cors";

import earthquakeAlertRoutes from "./routes/earthquakealert.js";

import rawDataRoutes from "./routes/rawdata.js";

const app = express();
// This should be declared under const app = express();
const setXContentTypeOptions = (req, res, next) => {
  res.set("x-content-type-options", "nosniff");
  next();
};
// This should be declared under the setXContentTypeOptions function
const setXFrameOptions = (req, res, next) => {
  res.set("x-frame-options", "deny");
  next();
};
const setContentSecurityPolicy = (req, res, next) => {
  res.set("content-security-policy", "default-src 'none'");
  next();
};

// This should be declared under app.use(cors());
app.use(setXContentTypeOptions);
// This should be declared under app.use(setXContentTypeOptions);
app.use(setXFrameOptions);
app.use(setContentSecurityPolicy);

// Use the CORS module
app.use(cors());
// This should be declared under app.use(cors());
app.use(express.urlencoded({ extended: false })); // To parse the incoming requests with urlencoded payloads. For example, form data
// This should be declared under app.use(urlencoded({ extended: false }));
app.use(express.json()); // To parse the incoming requests with JSON payloads. For example, REST API requests

app.use("/api/earthquake-alerts", earthquakeAlertRoutes);
app.use("/api/raw-data", rawDataRoutes);


app.get("*", (req, res) => {
    res.status(404).json({ msg: "Endpoint does not exist" });
  });
  
  // Start the server on port 3000
  app.listen(8080, () => {
    console.log("Server is listening on port 8080.");
  });
  
  // Export the Express application. Other modules may use it. For example, API testing
  export default app;