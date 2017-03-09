#pragma once
#include <switch.h>
#include <switch_dictionary.h>
#include "runtime.h"

namespace Trinity
{
        // Materialized hits for a term and the current document
        // this is used both for evaluation and for scoring documents
        struct term_hits
        {
                term_hit *all{0};
                uint16_t freq;
                uint16_t allCapacity{0};
		// facilitates execution -- ignoring during scoring
		uint16_t docSeq;

                void set_freq(const uint16_t newFreq)
                {
                        if (newFreq > allCapacity)
                        {
                                allCapacity = newFreq + 32;
                                if (all)
                                        std::free(all);
                                all = (term_hit *)std::malloc(sizeof(term_hit) * allCapacity);
                        }

                        freq = newFreq;
                }

                ~term_hits()
                {
                        if (all)
                                std::free(all);
                }
        };

	// We record an instance for each term instances in a original/input query
        struct query_term_instances
        {
		struct 
		{
			exec_term_id_t id;
			strwlen8_t token;
		} term;

                uint8_t cnt;
                struct instance_struct
                {
                        // TODO: flags?
                        uint8_t index;
                        uint8_t rep;
                } instances[0];
        };

        struct matched_query_term
        {
                // Every instance of the term in the query
                // the token may be used more than once
                // and each time a different rep/index may be set
                const query_term_instances *queryTermInstances;	// see runtime_ctx.originalQueryTermInstances
                term_hits *hits;     				// all hits in current document for this term. See runtime_ctx.termHits
        };


	// Score functions are provided with a matched_document
	// and are expected to return a score 
        struct matched_document
        {
                uint32_t id; // document ID
                uint16_t matchedTermsCnt;
                matched_query_term *matchedTerms;
        };
}
