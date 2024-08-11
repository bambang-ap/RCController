const fs = require('fs')

const ino = fs.readFileSync('./RCController.ino')

const inoArray = ino.toString().split('\n')
const startIndex = inoArray.findIndex(p => p.includes('// Start html'))
const endIndex = inoArray.findIndex(p => p.includes('// End html'))

const start = inoArray.slice(0, startIndex + 1)
const end = inoArray.slice(endIndex)
const html = "12345"

const finalScripts = [...start, html, ...end].join("\n")

console.log(finalScripts)