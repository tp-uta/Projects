import mongoose from 'mongoose';

const Schema = mongoose.Schema;


const ocgSchema = new Schema({
    title: {
        type: String,
        required: true
    },
    data: {
        type: Object,
        required: true
    },
    report_num: {
        type: String,
        required: true,
        unique: true,
    }
}, { timestamps: true });

const OCG = mongoose.model('ocg', ocgSchema);
export {OCG};
