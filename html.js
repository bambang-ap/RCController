const fs = require('fs')

const ino = fs.readFileSync('./RCController.ino')
const html = fs.readFileSync('./page.min.html')

const inoArray = ino.toString().split('\n')
const startIndex = inoArray.findIndex(p => p.includes('// Start html'))
const endIndex = inoArray.findIndex(p => p.includes('// End html'))

const start = inoArray.slice(0, startIndex + 1)
const end = inoArray.slice(endIndex)
const formattedHtml = html
	.toString()
	.split('\n')
	.map(line => {
		const d = line.replace(/\"/g, "\\\"")
		return `\tclient.println("${d.slice(0, -1)}");`
	})

const finalScripts = [...start, ...formattedHtml, ...end].join("\n")

fs.writeFileSync('./RCController.ino', finalScripts)