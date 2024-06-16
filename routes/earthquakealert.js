import express from "express";

import {
  createEarthquakeAlert,
  getEarthquakeAlerts,
  getEarthquakeAlert,
  updateEarthquakeAlert,
  deleteEarthquakeAlert,
} from "../controllers/earthquakealert.js";

const router = express.Router();

router.post("/", createEarthquakeAlert);
router.get("/", getEarthquakeAlerts);
router.get("/:id", getEarthquakeAlert);
router.put("/:id", updateEarthquakeAlert);
router.delete("/:id", deleteEarthquakeAlert);

export default router;