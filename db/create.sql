spool create_table.log
prompt
prompt Creating table TG_ACCMAP
prompt =========================
prompt

drop table TG_ACCMAP;
create table TG_ACCMAP
(
  refid       CHAR(32)     not null,
  platformid  CHAR(8)      not null,
  accountcode NUMBER(19)   not null,
  acccode     CHAR(19)     not null,
  cardid      NUMBER(19)   not null,
  trackcardno VARCHAR2(18) not null,
  custid      NUMBER(19)   not null,
  status      CHAR(1)      not null,
  createdate  CHAR(8)      not null
)
;
comment on table TG_ACCMAP
  is '托管业务账号映射表';
comment on column TG_ACCMAP.refid is '外部标识号';
comment on column TG_ACCMAP.platformid is '平台号';
comment on column TG_ACCMAP.accountcode is '账户代码';
comment on column TG_ACCMAP.acccode is '子账户代码';
comment on column TG_ACCMAP.cardid is '卡片id';
comment on column TG_ACCMAP.trackcardno is '卡面卡号';
comment on column TG_ACCMAP.custid is '客户号';
comment on column TG_ACCMAP.status is '状态';
comment on column TG_ACCMAP.createdate is '创建日期';

alter table TG_ACCMAP
  add constraint PK_TG_ACCMAP primary key (refid, platformid);
  
drop table TG_ORGANINFO;
create table TG_ORGANINFO
(
  instcode       CHAR(8)      not null,
  platformid     CHAR(8)      not null,
  instname       VARCHAR2(80),
  linkname       VARCHAR2(20),
  address        VARCHAR2(120),
  tel            VARCHAR2(40),
  zipcode        VARCHAR2(6),
  email          VARCHAR2(40),
  remark         VARCHAR2(40),
  status         CHAR(2)     not null,
  createdate     CHAR(8)     not null
)
;
comment on table TG_ORGANINFO
  is '托管业务机构信息表';
comment on column TG_ORGANINFO.instcode   is '机构号';
comment on column TG_ORGANINFO.platformid is '平台号';
comment on column TG_ORGANINFO.instname   is '机构名';
comment on column TG_ORGANINFO.linkname   is '联系人';
comment on column TG_ORGANINFO.address    is '地址';
comment on column TG_ORGANINFO.tel        is '电话';
comment on column TG_ORGANINFO.zipcode    is '邮编';
comment on column TG_ORGANINFO.email      is 'email';
comment on column TG_ORGANINFO.remark     is '备注';
comment on column TG_ORGANINFO.status     is '机构状态';
comment on column TG_ORGANINFO.createdate is '创建日期';

alter table TG_ORGANINFO
  add constraint PK_TG_ORGANINFO primary key (instcode, platformid);
  
drop table TG_INSTACCINFO;
create table TG_INSTACCINFO
(
  instcode       CHAR(8)      not null,
  platformid     CHAR(8)      not null,
  rechargetotamt NUMBER(18,0) not null,
  rechargetotnum NUMBER(8)    not null,
  transouttotamt NUMBER(18,0) not null,
  transouttotnum NUMBER(8)    not null,
  transintotamt  NUMBER(18,0) not null,
  transintotnum  NUMBER(8)    not null,
  feetotamt      NUMBER(18,0) not null,
  feetotnum      NUMBER(8)    not null,
  amout          NUMBER(18,0) not null,
  balancemd5     CHAR(32)     not null
)
;
comment on table TG_INSTACCINFO
  is '托管业务机构账户表(信托)';
comment on column TG_INSTACCINFO.instcode       is '机构号';
comment on column TG_INSTACCINFO.platformid     is '平台号';
comment on column TG_INSTACCINFO.rechargetotamt is '累计充值总金额';
comment on column TG_INSTACCINFO.rechargetotnum is '累计充值总笔数';
comment on column TG_INSTACCINFO.transouttotamt is '累计转出总金额';
comment on column TG_INSTACCINFO.transouttotnum is '累计转出总笔数';
comment on column TG_INSTACCINFO.transintotamt  is '累计转入总金额';
comment on column TG_INSTACCINFO.transintotnum  is '累计转入总笔数';
comment on column TG_INSTACCINFO.feetotamt      is '累计咨询费总金额';
comment on column TG_INSTACCINFO.feetotnum      is '累计咨询费总笔数';
comment on column TG_INSTACCINFO.amout          is '可用余额';
comment on column TG_INSTACCINFO.balancemd5     is '余额的md5检验';

alter table TG_INSTACCINFO
  add constraint PK_TG_INSTACCINFO primary key (instcode, platformid);
  
drop table TG_PLATACCINFO;
create table TG_PLATACCINFO
(
  platformid      CHAR(8)      not null,
  rechargetotamt  NUMBER(18,0) not null,
  rechargetotnum  NUMBER(8)    not null,
  cashtotamt      NUMBER(18,0) not null,
  cashtotnum      NUMBER(8)    not null,
  incometotamt    NUMBER(18,0) not null,
  incometotnum    NUMBER(8)    not null,
  extractedtotamt NUMBER(18,0) not null,
  extractedtotnum NUMBER(8)    not null,
  feeamount       NUMBER(18,0) not null,
  feebalancemd5   CHAR(32)     not null,
  amout           NUMBER(18,0) not null,
  balancemd5      CHAR(32)     not null
)
;
comment on table TG_PLATACCINFO
  is '托管业务平台账户表';
comment on column TG_PLATACCINFO.platformid      is '平台号';
comment on column TG_PLATACCINFO.rechargetotamt  is '累计充值总金额';
comment on column TG_PLATACCINFO.rechargetotnum  is '累计充值总笔数';
comment on column TG_PLATACCINFO.cashtotamt      is '累计提现总金额';
comment on column TG_PLATACCINFO.cashtotnum      is '累计提现总笔数';
comment on column TG_PLATACCINFO.incometotamt    is '累计收益总金额';
comment on column TG_PLATACCINFO.incometotnum    is '累计收益总笔数';
comment on column TG_PLATACCINFO.extractedtotamt is '已提取收益总金额';
comment on column TG_PLATACCINFO.extractedtotnum is '已提取收益总笔数';
comment on column TG_PLATACCINFO.feeamount       is '收益余额';
comment on column TG_PLATACCINFO.feebalancemd5   is '收益余额的md5检验';
comment on column TG_PLATACCINFO.amout           is '余额';
comment on column TG_PLATACCINFO.balancemd5      is '余额的md5检验';

alter table TG_PLATACCINFO
  add constraint PK_TG_PLATACCINFO primary key (platformid);
  
drop table TG_PLATFORMINFO;
create table TG_PLATFORMINFO
(
  platformid     CHAR(8)      not null,
  platformtype   CHAR(2)      not null,
  platformname   VARCHAR2(80) not null,
  linkname       VARCHAR2(20),
  address        VARCHAR2(120),
  tel            VARCHAR2(40),
  zipcode        VARCHAR2(6),
  email          VARCHAR2(40),
  remark         VARCHAR2(40),
  status         CHAR(2)      not null,
  createdate     CHAR(8)      not null
)
;
comment on table TG_PLATFORMINFO
  is '托管业务机构信息表';
comment on column TG_PLATFORMINFO.platformid   is '平台号';
comment on column TG_PLATFORMINFO.platformtype is '平台类型';
comment on column TG_PLATFORMINFO.platformname is '平台名称';
comment on column TG_PLATFORMINFO.linkname     is '联系人';
comment on column TG_PLATFORMINFO.address      is '地址';
comment on column TG_PLATFORMINFO.tel          is '电话';
comment on column TG_PLATFORMINFO.zipcode      is '邮编';
comment on column TG_PLATFORMINFO.email        is 'email';
comment on column TG_PLATFORMINFO.remark       is '备注';
comment on column TG_PLATFORMINFO.status       is '机构状态';
comment on column TG_PLATFORMINFO.createdate   is '创建日期';

alter table TG_PLATFORMINFO
  add constraint PK_TG_PLATFORMINFO primary key (platformid);
  
drop table TG_ACCOUNTFLOWP;
create table TG_ACCOUNTFLOWP
(
  referenceno  CHAR(18)     not null,
  paltformid   CHAR(8)      not null,
  transcode    CHAR(4)      not null,
  reftxnno     CHAR(5)      not null,
  channelcode  CHAR(8)      not null,
  termcode     CHAR(8)      not null,
  accountid    NUMBER(19)   not null,
  accountname  VARCHAR2(20),
  linkaccid    NUMBER(19),
  linkaccname  VARCHAR2(20),
  acccode      CHAR(19)     not null,
  linkacccode  CHAR(19),
  commdate     CHAR(8),
  commtime     CHAR(6),
  transdate    CHAR(8),
  transtime    CHAR(6),
  amount       NUMBER(18,0) not null,
  fee          NUMBER(18,0) not null,
  status       CHAR(2)      not null,
  paycode      CHAR(2)      not null,
  txnid        CHAR(16)     not null
)
;
comment on table TG_ACCOUNTFLOWP
  is '托管业务账务流水表(个人)';
comment on column TG_ACCOUNTFLOWP.referenceno is '流水号';
comment on column TG_ACCOUNTFLOWP.paltformid  is '平台号';
comment on column TG_ACCOUNTFLOWP.transcode   is '交易代码';
comment on column TG_ACCOUNTFLOWP.reftxnno    is '外部交易代码';
comment on column TG_ACCOUNTFLOWP.channelcode is '渠道代码';
comment on column TG_ACCOUNTFLOWP.termcode    is '终端代码';
comment on column TG_ACCOUNTFLOWP.accountid   is '账户代码';
comment on column TG_ACCOUNTFLOWP.accountname is '账户名';
comment on column TG_ACCOUNTFLOWP.linkaccid   is '关联账户代码';
comment on column TG_ACCOUNTFLOWP.linkaccname is '关联账户名';
comment on column TG_ACCOUNTFLOWP.acccode     is '子账户代码';
comment on column TG_ACCOUNTFLOWP.linkacccode is '关联子账户代码';
comment on column TG_ACCOUNTFLOWP.commdate    is '通讯日期';
comment on column TG_ACCOUNTFLOWP.commtime    is '通讯时间';
comment on column TG_ACCOUNTFLOWP.transdate   is '交易日期';
comment on column TG_ACCOUNTFLOWP.transtime   is '交易时间';
comment on column TG_ACCOUNTFLOWP.amount      is '交易金额';
comment on column TG_ACCOUNTFLOWP.fee         is '咨询费';
comment on column TG_ACCOUNTFLOWP.status      is '交易状态';
comment on column TG_ACCOUNTFLOWP.paycode     is '结算方式代码';
comment on column TG_ACCOUNTFLOWP.txnid       is '外部系统流水号';

alter table TG_ACCOUNTFLOWP
  add constraint PK_TG_ACCOUNTFLOWP primary key (referenceno);
  
drop table TG_ACCOUNTFLOWO;
create table TG_ACCOUNTFLOWO
(
  referenceno  CHAR(18)     not null,
  paltformid   CHAR(8)      not null,
  instcode     CHAR(8)      not null,
  transcode    CHAR(4)      not null,
  reftxnno     CHAR(5)      not null,
  channelcode  CHAR(8)      not null,
  termcode     CHAR(8)      not null,
  accountid    NUMBER(19)   not null,
  accountname  VARCHAR2(20),
  linkaccid    NUMBER(19),
  linkaccname  VARCHAR2(20),
  acccode      CHAR(19)     not null,
  linkacccode  CHAR(19),
  commdate     CHAR(8),
  commtime     CHAR(6),
  transdate    CHAR(8),
  transtime    CHAR(6),
  amount       NUMBER(18,0) not null,
  fee          NUMBER(18,0) not null,
  status       CHAR(2)      not null,
  paycode      CHAR(2)      not null,
  txnid        CHAR(16)     not null
)
;
comment on table TG_ACCOUNTFLOWO
  is '托管业务账务流水表(机构)';
comment on column TG_ACCOUNTFLOWO.referenceno is '流水号';
comment on column TG_ACCOUNTFLOWO.paltformid  is '平台号';
comment on column TG_ACCOUNTFLOWO.instcode    is '机构号';
comment on column TG_ACCOUNTFLOWO.transcode   is '交易代码';
comment on column TG_ACCOUNTFLOWO.reftxnno    is '外部交易代码';
comment on column TG_ACCOUNTFLOWO.channelcode is '渠道代码';
comment on column TG_ACCOUNTFLOWO.termcode    is '终端代码';
comment on column TG_ACCOUNTFLOWO.accountid   is '账户代码';
comment on column TG_ACCOUNTFLOWO.accountname is '账户名';
comment on column TG_ACCOUNTFLOWO.linkaccid   is '关联账户代码';
comment on column TG_ACCOUNTFLOWO.linkaccname is '关联账户名';
comment on column TG_ACCOUNTFLOWO.acccode     is '子账户代码';
comment on column TG_ACCOUNTFLOWO.linkacccode is '关联子账户代码';
comment on column TG_ACCOUNTFLOWO.commdate    is '通讯日期';
comment on column TG_ACCOUNTFLOWO.commtime    is '通讯时间';
comment on column TG_ACCOUNTFLOWO.transdate   is '交易日期';
comment on column TG_ACCOUNTFLOWO.transtime   is '交易时间';
comment on column TG_ACCOUNTFLOWO.amount      is '交易金额';
comment on column TG_ACCOUNTFLOWO.fee         is '咨询费';
comment on column TG_ACCOUNTFLOWO.status      is '交易状态';
comment on column TG_ACCOUNTFLOWO.paycode     is '结算方式代码';
comment on column TG_ACCOUNTFLOWO.txnid       is '外部系统流水号';

alter table TG_ACCOUNTFLOWO
  add constraint PK_TG_ACCOUNTFLOWO primary key (referenceno);


create sequence SEQ_TG_LS
minvalue 1
maxvalue 99999999
start with 1
increment by 1
nocache
cycle;

spool off
