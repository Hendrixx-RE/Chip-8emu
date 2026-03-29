# CHIP-8 Emulator

A  CHIP-8 emulator written in **C++** with a terminal-based interface using **notcurses**.

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![Interface](https://img.shields.io/badge/UI-notcurses-green.svg)
![License](https://img.shields.io/badge/license-MIT-blue)

---

### Prerequisites
You will need a C++ compiler (GCC or Clang) and the ncurses development libraries installed on your system.
```bash
sudo pacman -S notcurses
```

### Build
```bash
git clone https://github.com/Hendrixx-RE/Chip-8emu.git
cd Chip-8emu
cmake -B build
cmake --build build
cd build
```
### Running
```bash
./Chip8 {game delay in ms (2)} {rom location relative to the game folder}
```
![demo](https://github.com/user-attachments/assets/ecdb79d3-5dd9-466c-afc5-57a8000fa1ae)


# Normal-Ice: AI-Powered Healthcare Claim & Data Management Platform

**Normal-Ice** is a comprehensive, production-ready healthcare management system designed to streamline clinical data extraction, case management, and financial auditing. By leveraging **Google Gemini AI (v2.5 Flash)** and **FastAPI**, it automates the transformation of unstructured medical documents into standardized **FHIR R4** resources, while providing a full suite of tools for hospital and insurance workflows.

---

##  Key Features

### 1. Clinical Data Extraction (OCR to FHIR)
- **Intelligent OCR**: Dual-strategy extraction using `PyMuPDF` for text-based PDFs and **`doctr` (python-doctr)** for scanned/image-based documents.
- **AI-Powered Structuring**: Uses Gemini 2.5 Flash to extract patient demographics, clinical findings, diagnoses, medications, and observations.
- **FHIR R4 Generation**: Automatically generates valid FHIR bundles (Patient, Practitioner, Observation, DiagnosticReport, MedicationRequest, etc.).
- **Batch Processing**: Supports large medical records (40+ pages) by intelligently splitting and merging results.

### 2. AI-Driven Document Evaluation & Normalization
- **Discrepancy Flagging**: AI-powered evaluation of medical documents to identify clinical discrepancies or missing documentation *before* reports are finalized.
- **Insurance Deduction Prediction**: Flags potential insurance deductions and claim risks based on IRDAI and TPA adjudication rules.
- **Billing Flags**: Real-time identification of patient identity mismatches, provider credential issues, and coding errors.

### 3. Financial Audit & Bill Verification
- **Claim Reconciliation**: Automated verification of the final bill against pre-authorization limits and intermediate enhancement requests.
- **Justification Engine**: Gemini-generated narrative audits that explain *why* specific costs were incurred, referencing clinical evidence from the documents.
- **Variance Analysis**: Tracks room charges, ICU, surgery, medicine, and investigation costs to flag overruns or unexplained charges.
- **Risk Assessment**: Categorizes cases by risk level (Low/Medium/High) based on billing integrity.

### 4. Case & Insurance Workflow
- **Centralized Case Tracking**: End-to-end lifecycle management from pre-auth to final settlement.
- **Pre-authorization & Enhancements**: Automated extraction and validation of insurance forms.
- **MIS & Dashboards**: Management Information System for tracking operational metrics, financial performance, and claim status.

---

## Architecture & Tech Stack

### Backend (Python/FastAPI)
- **Framework**: FastAPI (Asynchronous, Type-safe)
- **AI Engine**: Google Generative AI (Gemini 2.5 Flash)
- **OCR Pipeline**: `python-doctr` (DocTR), `PyMuPDF`
- **Database**: Supabase (PostgreSQL + Auth)
- **Standards**: `fhir.resources` (FHIR R4), Pydantic v2

### Frontend (React/TypeScript)
- **Framework**: React 18 with TypeScript
- **Styling**: Tailwind CSS (with Dark Mode support)
- **State Management**: React Hooks
- **Icons**: Lucide React

---

## Project Structure

```text
Normal-Ice/
├── backend/                # FastAPI Application
│   ├── app/
│   │   ├── routes/         # API Endpoints (cases, financial_audit, patients, etc.)
│   │   ├── services/       # Business Logic (OCR, LLM, Financial Auditor, Claim Reconciliation)
│   │   ├── models/         # Pydantic Schemas & Data Models
│   │   └── data/           # Reference Data (Cost Estimates, Audit Rules)
├── frontend/               # React Application
│   ├── src/
│   │   ├── components/     # UI Components (FinanceManagerPage, CasePage, FileUpload, etc.)
│   │   ├── services/       # API Integration (Axios)
│   │   └── types/          # TypeScript Definitions
└── dummy_data/             # Sample Medical PDFs for Testing
```

---

## Data Processing Workflow

1.  **Upload & OCR**: PDF is processed via `PyMuPDF` or `doctr` to extract raw text.
2.  **AI Normalization**: Gemini structures the text and flags clinical discrepancies or potential billing issues.
3.  **FHIR Mapping**: Data is converted into standardized FHIR R4 bundles.
4.  **Financial Audit**: Upon discharge, the final bill is automatically reconciled against pre-auth and enhancement history.
5.  **Review**: Finance managers review the AI-generated audit reports and recommendations before finalization.

---

## Installation & Setup

### Prerequisites
- Python 3.10+
- Node.js 18+
- Google Gemini API Key
- Supabase Account

### 1. Backend Setup
```bash
cd backend
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate




