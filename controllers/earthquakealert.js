/**
 * @file Manages all operations related to earthquakes
 * @author Ben Mahoney
 */

import { PrismaClient } from "@prisma/client";
import { raw } from "@prisma/client/runtime/library";
import e from "express";

const prisma = new PrismaClient();



const createEarthquakeAlert = async (req, res) => {
  try {
    const contentType = req.headers["content-type"];
    if (!contentType || contentType !== "application/json") {
      return res.status(400).json({
        msg: "Invalid Content-Type. Expected application/json.",
      });
    }

    await prisma.earthquakeAlert.create({
      data: { ...req.body },
    });

    const newEarthquakeAlerts = await prisma.earthquakeAlert.findMany();

    return res.status(201).json({
      msg: "Alert successfully saved",
      data: newEarthquakeAlerts,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const getEarthquakeAlerts = async (req, res) => {
    try {
    const earthquakeAlerts = await prisma.earthquakeAlert.findMany(); // Fetch earthquake alert from the database using Prisma

    if (earthquakeAlerts.length === 0) {
      return res.status(404).json({ msg: "No alerts found" });
    }

    return res.json({ data: earthquakeAlerts });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const getEarthquakeAlert = async (req, res) => {
  try {
    const earthquakeAlert = await prisma.earthquakeAlert.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!earthquakeAlert) {
      return res
        .status(404)
        .json({ msg: `No alert with the id: ${req.params.id} found` });
    }

    return res.json({
      data: earthquakeAlert,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const updateEarthquakeAlert = async (req, res) => {
  try {
    const contentType = req.headers["content-type"];
    if (!contentType || contentType !== "application/json") {
      return res.status(400).json({
        msg: "Invalid Content-Type. Expected application/json.",
      });
    }

    let earthquakeAlert = await prisma.earthquakeAlert.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!earthquakeAlert) {
      return res
        .status(404)
        .json({ msg: `Alert with id: ${req.params.id} not found` });
    }

    earthquakeAlert = await prisma.earthquakeAlert.update({
      where: { id: Number(req.params.id) },
      data: { ...req.body },
    });

    return res.json({
      msg: `Alert with the id: ${req.params.id} successfully updated`,
      data: earthquakeAlert,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const deleteEarthquakeAlert = async (req, res) => {
  try {
    const earthquakeAlert = await prisma.earthquakeAlert.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!earthquakeAlert) {
      return res
        .status(404)
        .json({ msg: `No alert with the id: ${req.params.id} found` });
    }

    await prisma.earthquakeAlert.delete({
      where: { id: Number(req.params.id) },
    });

    return res.json({
      msg: `Alert with the id: ${req.params.id} successfully deleted`,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

export {
  createEarthquakeAlert,
  getEarthquakeAlert,
  getEarthquakeAlerts,
  updateEarthquakeAlert,
  deleteEarthquakeAlert,
};