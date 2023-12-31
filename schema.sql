CREATE TABLE  `TASK`
(
    `UUID`            varchar(36) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
    `CYCLEID`            INT       NOT NULL,
    `SATID`            INT       NOT NULL,
    `PAYLOADID`            INT       NOT NULL,
    `WAVEBANDID`            INT       NOT NULL,
    `INTEGRALTIME`    decimal(10,2),
   `STARTTIMESTAMP`   TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `ENDTIMESTAMP`     TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `ENTRYTIME`        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `STATUS` INT NOT NULL,
    PRIMARY KEY (`UUID`) ,
    index(`CYCLEID`)
);

CREATE TABLE  `GENERICIMAGE`
(
    `UUID`             VARCHAR(36)  NOT NULL,
    `PROCESSSTATIONID` INT          ,
    `TASKID`            INT       DEFAULT 0,
    `SATID`            INT       NOT NULL,
    `PAYLOADID`        INT,
    `WAVEBANDID`       INT,
    `INTEGRALTIME`    decimal(10,2),
    `CHANNELID`        INT,
    `STARTTIMESTAMP`   TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `ENDTIMESTAMP`     TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `STARTFRAMECOUNT`  INT,
    `ENDFRAMECOUNT`    INT,
    `STAREWORKMODE`    INT,
    `GENTIME`          TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `SUBFILELEN`       INT,
    `SUBFILENUM`       INT,
    `REMOTEFILEPATH`         VARCHAR(255),
    `LOCALFILEPATH`         VARCHAR(255),
    `LIFESTATUS`       INT,
    `ENTRYTIME`        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `PAYLOADSTATUS`    INT,
    PRIMARY KEY (`UUID`) ,
    index(`TASKID`),
    index(`STARTTIMESTAMP`)
);


CREATE TABLE  `PROCESSPATTERN`
(
    `UUID`             VARCHAR(36)  NOT NULL,
    `SATID`            INT       NOT NULL,
    `PAYLOADID`        INT,
    `WAVEBANDID`       INT,
    `INTEGRALTIME`    decimal(10,2),
    `MODULE`       VARCHAR(64)  NOT NULL,
    `ALGORITHM`       VARCHAR(64)  NOT NULL,
    `PARAMETERS`  VARCHAR(1024)  NOT NULL,
    `CONDITIONS`  VARCHAR(1024)  NOT NULL,
    `ENTRYTIME`        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `STATUS` INT NOT NULL,
    PRIMARY KEY (`UUID`) ,
    index(`MODULE`)
);


CREATE TABLE  `SUBTASK`
(
    `UUID`             VARCHAR(36)  NOT NULL,
    `TASKID`             VARCHAR(36)  NOT NULL,
    `PROCESSPATTERNID`       VARCHAR(36)  NOT NULL,
    `STEP`       VARCHAR(36)  NOT NULL,
     `STATUS` INT NOT NULL,
    `ENTRYTIME`        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`UUID`) ,
     index(`STATUS`),
    index(`TASKID`)
);


CREATE TABLE  `PROCESSRECORD`
(
    `UUID`             VARCHAR(36)  NOT NULL,
    `GENERICIMAGEID`            INT       NOT NULL,
    `TASKID`            INT       NOT NULL,
    `LOCALFILEPATH`         VARCHAR(255),
    `ENTRYTIME`        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`UUID`) ,
    index(`GENERICIMAGEID`)
);
