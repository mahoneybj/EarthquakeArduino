/**
 * @file Manages all operations related to earthquakes
 * @author Ben Mahoney
 */

import { PrismaClient } from "@prisma/client";
import { raw } from "@prisma/client/runtime/library";

const prisma = new PrismaClient();

const paginationDefault = {
  amount: 25, // The number of items per page
  page: 1, // The page number
};

const createRawData = async (req, res) => {
  try {
    const contentType = req.headers["content-type"];
    if (!contentType || contentType !== "application/json") {
      return res.status(400).json({
        msg: "Invalid Content-Type. Expected application/json.",
      });
    }

    await prisma.rawData.create({
      data: { ...req.body },
    });

    const newRawDatas = await prisma.rawData.findMany();

    return res.status(201).json({
      msg: "Raw data successfully saved",
      data: newRawDatas,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const getRawDatas = async (req, res) => {
    try {
    const query = {
      take: Number(amount),
      skip: (Number(page) - 1) * Number(amount),
      orderBy: {
        [sortBy]: sortOrder,
      },
    };

    const rawDatas = await prisma.rawData.findMany(query); // Fetch rawdata data from the database using Prisma

    if (rawDatas.length === 0) {
      return res.status(404).json({ msg: "No data found" });
    }

    return res.json({ data: rawDatas });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const getRawData = async (req, res) => {
  try {
    const rawData = await prisma.rawData.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!rawData) {
      return res
        .status(404)
        .json({ msg: `No data with the id: ${req.params.id} found` });
    }

    return res.json({
      data: rawData,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const updateRawData = async (req, res) => {
  try {
    const contentType = req.headers["content-type"];
    if (!contentType || contentType !== "application/json") {
      return res.status(400).json({
        msg: "Invalid Content-Type. Expected application/json.",
      });
    }

    let rawData = await prisma.rawData.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!rawData) {
      return res
        .status(404)
        .json({ msg: `Data with id: ${req.params.id} not found` });
    }

    rawData = await prisma.rawData.update({
      where: { id: Number(req.params.id) },
      data: { ...req.body },
    });

    return res.json({
      msg: `Data with the id: ${req.params.id} successfully updated`,
      data: rawData,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

const deleteRawData = async (req, res) => {
  try {
    const rawData = await prisma.rawData.findUnique({
      where: { id: Number(req.params.id) },
    });

    if (!rawData) {
      return res
        .status(404)
        .json({ msg: `No data with the id: ${req.params.id} found` });
    }

    await prisma.rawData.delete({
      where: { id: Number(req.params.id) },
    });

    return res.json({
      msg: `Data with the id: ${req.params.id} successfully deleted`,
    });
  } catch (err) {
    return res.status(500).json({
      msg: err.message,
    });
  }
};

export {
  createRawData,
  getRawData,
  getRawDatas,
  updateRawData,
  deleteRawData,
};