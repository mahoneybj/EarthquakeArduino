-- CreateTable
CREATE TABLE "RawData" (
    "id" SERIAL NOT NULL,
    "timestamp" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "x" DECIMAL(65,30) NOT NULL,
    "y" DECIMAL(65,30) NOT NULL,
    "z" DECIMAL(65,30) NOT NULL,

    CONSTRAINT "RawData_pkey" PRIMARY KEY ("id")
);

-- CreateTable
CREATE TABLE "EarthquakeAlert" (
    "id" SERIAL NOT NULL,
    "timestamp" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "pwave" DECIMAL(65,30) NOT NULL,
    "swave" DECIMAL(65,30) NOT NULL,
    "sensorid" INTEGER NOT NULL,
    "active" BOOLEAN NOT NULL,

    CONSTRAINT "EarthquakeAlert_pkey" PRIMARY KEY ("id")
);
