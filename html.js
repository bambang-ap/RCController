const fs = require('fs')

const ino = fs.readFileSync('./HtmlSender.ino')
const html = fs.readFileSync('./page2.html')

const inoArray = ino.toString().split('\n')
const startIndex = inoArray.findIndex(p => p.includes('// Start html'))
const endIndex = inoArray.findIndex(p => p.includes('// End html'))

const start = inoArray.slice(0, startIndex + 1)
const end = inoArray.slice(endIndex)
const formattedHtml = html
	.toString()
	.split('\n')
	.map(line => {
		const d = line
			.replace(/\"/g, "\\\"")
			.replace(/\r|\t/g, "")
		if (!d) return false;
		return `\tptr += "${d}\\n";`
	})
	.filter(Boolean)

const finalScripts = [...start, ...formattedHtml, ...end].join("\n")

fs.writeFileSync('./HtmlSender.ino', finalScripts)