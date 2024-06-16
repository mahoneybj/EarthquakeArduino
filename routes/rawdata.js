import express from "express";

import {
  createRawData,
  getRawDatas,
  getRawData,
  updateRawData,
  deleteRawData,
} from "../controllers/rawdata.js";

const router = express.Router();

router.post("/", createRawData);
router.get("/", getRawDatas);
router.get("/:id", getRawData);
router.put("/:id", updateRawData);
router.delete("/:id", deleteRawData);

export default router;