import express from 'express';
import * as reportController from '../controller/reportController.js'
import {requireAuth} from '../middleware/authMiddleware.js'

const reportRouter = express.Router();

//The report pages
reportRouter.get('/OCGAnalysis/:id', reportController.report_pages);

//Page to submit a URL to scrape to get a report
reportRouter.get('/report/create', requireAuth, reportController.report_create_page);

//Analysis API for pages to get up-to-date data
reportRouter.get('/report/:id', reportController.report_get);

//Set API to accept URLs for Python Web Scraping
reportRouter.post('/give', reportController.report_create_post)

export { reportRouter }